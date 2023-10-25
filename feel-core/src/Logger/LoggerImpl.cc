#include <ctime>
#include <error.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdarg.h>
#include <algorithm>

#include <regex>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>

#include "Logger.hh"
#include "LoggerImpl.hh"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#ifdef VERBOSE
extern int errno;
#endif

const char SYSTEM_PATH_DELIMITER =
    #if defined _WIN32 || defined __CYGWIN__
  '\\';
#else
    '/';
#endif

static inline long long int epochTimeStamp() {
  struct timespec tms;
  if (-1 == clock_gettime(CLOCK_REALTIME, &tms))
  {
    return 0;
  }

  // only care about seconds
  return (long int)(tms.tv_sec * (long int)1e6) + (long int)(tms.tv_nsec / 1000);
}

static long int getFileSize(const string& absPathToFile) {
  struct stat st;
  if (0 != stat(absPathToFile.c_str(), &st)) {
    fprintf(stderr, "getFileSize: failed to stat() file %s\n", absPathToFile.c_str());
    return 0;
  }
  return st.st_size;
}

static ssize_t copyFile(const string& source, const string& dest) {
  int res_source = open(source.c_str(), O_RDONLY, 0);
  if (-1 == res_source) {
    fprintf(stderr, "copyFile: failed to open source file %s\n", source.c_str());
#ifdef VERBOSE
    fprintf(stderr, "copyFile: errno %d, errmsg: %s\n", errno, strerror(errno));
#endif
    return -1;
  }

  int res_dest = open(dest.c_str(), O_WRONLY | O_CREAT, 0644);
  if (-1 == res_dest) {
    fprintf(stderr, "copyFile: failed to open dest file %s\n", dest.c_str());
#ifdef VERBOSE
    fprintf(stderr, "copyFile: errno %d, errmsg: %s\n", errno, strerror(errno));
#endif
    return -1;
  }

  struct stat st;
  fstat(res_source, &st);
  ssize_t copied_bytes = sendfile(res_dest, res_source, 0, (size_t)st.st_size);
  close(res_dest);
  close(res_source);
  return copied_bytes;
}

static string buildFullFilePath(string& filePath, string& fileName) {
  auto fullPath = filePath;
  if (fullPath.size() != 0) {
    fullPath += SYSTEM_PATH_DELIMITER;
  }
  fullPath += fileName;
  return fullPath;
}

Logger& Logger::getLogger()
{
  static LoggerImpl logger;
  return logger;
}

LoggerImpl::LoggerImpl() :
    Logger(),
    m_log_buffer(new LogBuffer()), m_plain_log_files(), m_json_log_files(),
    m_plain_incident_log(nullptr), m_json_incident_log(nullptr),
    m_log_option(LogOption::Console), m_is_plain_log_enable(false),
    m_is_json_log_enable(false), m_separated_log(false),
    m_file_output_path(""), m_backup_log_dir(""), m_max_number_backup_files(0),
    m_file_size_limit(0), m_minimum_log_level(LogLevel::Debug),
    m_is_log_file_set(false)
{
  cleanLogFileMap();
}

LoggerImpl::~LoggerImpl()
{
  if (m_log_buffer)
    delete m_log_buffer;
}

void LoggerImpl::enablePlainLog(bool enable)
{
  m_is_plain_log_enable = enable;
}

void LoggerImpl::enableJsonLog(bool enable)
{
  m_is_json_log_enable = enable;
}

bool LoggerImpl::setLogFile(string filePath, string fileName)
{
  if (!fileName.size()) {
    fprintf(stderr, "Log file name is missing\n");
    return false;
  }

  cleanLogFileMap();
  m_file_output_path = filePath;

  if (m_is_plain_log_enable) {
    if (!m_plain_log_files[fileName + ".dev"]) {
      m_plain_log_files[fileName + ".dev"] = std::make_shared<LogFile>();
    }
    m_plain_log_files[fileName + ".dev"]->file_name = fileName + ".dev";

    if (!updateOutputFileStream(m_plain_log_files[fileName + ".dev"])) {
      m_plain_log_files[fileName + ".dev"]->file_name = "";
      m_file_output_path = "";
      return false;
    }
  }

  if (m_is_json_log_enable) {
    if (!m_json_log_files[fileName]) {
      m_json_log_files[fileName] = std::make_shared<LogFile>();
    }
    m_json_log_files[fileName]->file_name = fileName;

    if (!updateOutputFileStream(m_json_log_files[fileName])) {
      m_json_log_files[fileName]->file_name = "";
      m_file_output_path = "";
      return false;
    }
  }

  m_separated_log = false;
  return m_separated_log;
}

bool LoggerImpl::setLogFile(string filePath, const TechnicalFileList& fileList,
    string incidentFileName)
{
  if (!incidentFileName.size()) {
    fprintf(stderr, "Incident log file name is missing\n");
    return false;
  }

  cleanLogFileMap();
  m_file_output_path = filePath;

  for (const auto& it : fileList) {
    if (m_is_plain_log_enable) {
      if (!m_plain_log_files[it.first + ".dev"]) {
        m_plain_log_files[it.first + ".dev"] = std::make_shared<LogFile>();
      }
      m_plain_log_files[it.first + ".dev"]->file_name = it.second + ".dev";

      if (!updateOutputFileStream(m_plain_log_files[it.first + ".dev"])) {
        m_plain_log_files[it.first + ".dev"]->file_name = "";
        m_file_output_path = "";
        return false;
      }
    }

    if (m_is_json_log_enable) {
      if (!m_json_log_files[it.first]) {
        m_json_log_files[it.first] = std::make_shared<LogFile>();
      }
      m_json_log_files[it.first]->file_name = it.second;

      if (!updateOutputFileStream(m_json_log_files[it.first])) {
        m_json_log_files[it.first]->file_name = "";
        m_file_output_path = "";
        return false;
      }
    }
  }

  if (m_is_plain_log_enable) {
    if (!m_plain_incident_log)
      m_plain_incident_log = std::make_shared<LogFile>();
    m_plain_incident_log->file_name = incidentFileName + ".dev";
    if (!updateOutputFileStream(m_plain_incident_log)) {
      m_file_output_path = "";
      return false;
    }
  }

  if (m_is_json_log_enable) {
    if (!m_json_incident_log)
      m_json_incident_log = std::make_shared<LogFile>();
    m_json_incident_log->file_name = incidentFileName;
    if (!updateOutputFileStream(m_json_incident_log)) {
      m_file_output_path = "";
      return false;
    }
  }

  m_separated_log = true;
  return m_separated_log;
}

void LoggerImpl::setBackupLogDir(string backupDir)
{
  m_backup_log_dir = backupDir;
}

void LoggerImpl::setNumberBackupFiles(const uint8_t no)
{
  m_max_number_backup_files = no;
}

void LoggerImpl::setLogFileSizeLimit(uint32_t size)
{
  m_file_size_limit = size;
}

void LoggerImpl::setLogLevel(level minumumLevel)
{
  m_minimum_log_level = minumumLevel;
}

void LoggerImpl::setLogOption(option op)
{
  m_log_option = op;
}

void LoggerImpl::setLogMessageLength(uint32_t length)
{
  std::unique_lock<std::mutex> l(m_log_writer_mutex);
  m_log_buffer->setMaxMessageLength(length);
}

void LoggerImpl::setContextFieldWidth(uint8_t width)
{
  std::unique_lock<std::mutex> l(m_log_writer_mutex);
  m_log_buffer->setContextWidth(width);
}

void LoggerImpl::Log(level logLevel, string additional_info, string message)
{
  Log(logLevel, additional_info.c_str(), message.c_str());
}

void LoggerImpl::Log(level logLevel, string additional_info, const char* format, ...)
{
  if (m_minimum_log_level <= logLevel && LogOption::Off != m_log_option) {
    va_list argumentList;
    va_start(argumentList, format);
    log(logLevel, additional_info.c_str(), format, argumentList);
    va_end(argumentList);
  }
}

void LoggerImpl::Log(level logLevel, const char* additional_info,
    const char* format, ...)
{
  if (m_minimum_log_level <= logLevel && LogOption::Off != m_log_option) {
    va_list argumentList;
    va_start(argumentList, format);
    log(logLevel, additional_info, format, argumentList);
    va_end(argumentList);
  }
}

void LoggerImpl::Log(level logLevel, const char* file, const char* function,
    int line, const char* format, ...)
{
  if (m_minimum_log_level <= logLevel && LogOption::Off != m_log_option) {
    va_list argumentList;
    va_start(argumentList, format);
    log(logLevel, file, function, line, format, argumentList);
    va_end(argumentList);
  }
}

void LoggerImpl::Log(level logLevel, const char* file, const char* function,
    int line, string message)
{
  Log(logLevel, file, function, line, message.c_str());
}

bool LoggerImpl::updateOutputFileStream(std::shared_ptr<LogFile> logFile)
{
  auto fullPath = buildFullFilePath(m_file_output_path, logFile->file_name);
  if (nullptr != logFile->file_stream) {
    logFile->file_stream->close();
    delete logFile->file_stream;
    logFile->file_stream = nullptr;
  }

  std::ofstream* newOfStream = new std::ofstream();
  newOfStream->open(fullPath, std::ios::app);
  if (!newOfStream->is_open()) {
    fprintf(stderr, "Failed to create new out file stream\n");
    return false;
  }

  if (newOfStream) {
    logFile->file_stream = newOfStream;
    m_is_log_file_set = true;
  }
  else {
    fprintf(stderr, "Failed to set log file: %s\n", fullPath.c_str());
    m_is_log_file_set = false;
  }

  return m_is_log_file_set;
}

void LoggerImpl::logTruncate(LogCategory category)
{
  std::unique_lock<std::mutex> l(m_log_writer_mutex);

  auto technical_f = [&]() {
    if (m_is_plain_log_enable) {
      for (auto& file : m_plain_log_files) {
        if (file.second->file_stream) {
          file.second->file_stream->flush();
          file.second->file_stream->close();
          delete file.second->file_stream;
          file.second->file_stream = nullptr;
        }
        copyFile(buildFullFilePath(m_file_output_path, file.second->file_name),
            buildFullFilePath(m_backup_log_dir, file.second->file_name));
        std::remove(buildFullFilePath(m_file_output_path,
                file.second->file_name).c_str());
        updateOutputFileStream(file.second);
      }
    }

    if (m_is_json_log_enable) {
      for (auto& file : m_json_log_files) {
        if (file.second->file_stream) {
          file.second->file_stream->close();
          delete file.second->file_stream;
          file.second->file_stream = nullptr;
        }
        copyFile(buildFullFilePath(m_file_output_path, file.second->file_name),
            buildFullFilePath(m_backup_log_dir, file.second->file_name));
        std::remove(buildFullFilePath(m_file_output_path,
                file.second->file_name).c_str());
        updateOutputFileStream(file.second);
      }
    }
  };

  auto incident_f = [&]() {
    if (m_is_plain_log_enable && m_plain_incident_log) {
      m_plain_incident_log->file_stream->flush();
      m_plain_incident_log->file_stream->close();
      delete m_plain_incident_log->file_stream;
      m_plain_incident_log->file_stream = nullptr;

      copyFile(buildFullFilePath(m_file_output_path, m_plain_incident_log->file_name),
          buildFullFilePath(m_backup_log_dir, m_plain_incident_log->file_name));
      std::remove(buildFullFilePath(m_file_output_path,
              m_plain_incident_log->file_name).c_str());
      updateOutputFileStream(m_plain_incident_log);
    }

    if (m_is_json_log_enable && m_json_incident_log) {
      m_json_incident_log->file_stream->flush();
      m_json_incident_log->file_stream->close();
      delete m_json_incident_log->file_stream;
      m_json_incident_log->file_stream = nullptr;

      copyFile(buildFullFilePath(m_file_output_path, m_json_incident_log->file_name),
          buildFullFilePath(m_backup_log_dir, m_json_incident_log->file_name));
      std::remove(buildFullFilePath(m_file_output_path,
              m_json_incident_log->file_name).c_str());
      updateOutputFileStream(m_json_incident_log);
    }
  };

  if (category == kTechnical) {
#ifdef VERBOSE
    fprintf(stdout, "[VERBOSE] logTruncate for TECHNICAL\n");
#endif
    technical_f();
  } else if (category == kIncident) {
#ifdef VERBOSE
    fprintf(stdout, "[VERBOSE] logTruncate for INCIDENT\n");
#endif
    incident_f();
  } else if (category == (kTechnical | kIncident)) {
#ifdef VERBOSE
    fprintf(stdout, "[VERBOSE] logTruncate for TECHNICAL & INCIDENT\n");
#endif
    technical_f();
    incident_f();
  } else {
    ;
  }
}

bool LoggerImpl::validateFileSize(std::shared_ptr<LogFile> logFile)
{
  if (nullptr == logFile->file_stream) {
    return false;
  }

  auto fullPath = buildFullFilePath(m_file_output_path, logFile->file_name);
  auto fileSize = getFileSize(fullPath);

  if (fileSize >= m_file_size_limit) {
    if (nullptr != logFile->file_stream) {
      logFile->file_stream->flush();
      logFile->file_stream->close();
      delete logFile->file_stream;
      logFile->file_stream = nullptr;
    }

    if (!backupFile(logFile)) {
      return false;
    }

    if (!updateOutputFileStream(logFile)) {
      return false;
    }
  }

  return true;
}

bool LoggerImpl::backupFile(std::shared_ptr<LogFile> logFile)
{
  auto backupFiles = std::map<uint8_t, string>{};
  auto filter = std::regex(logFile->file_name + BACKUP_FILE_EXTENSION);
  DIR* dir;
  struct dirent* ent;
  if ((dir = opendir(m_backup_log_dir.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      auto sm = std::smatch();
      auto file = string(ent->d_name);
      if (std::regex_match(file, sm, filter)) {
        backupFiles.insert(std::make_pair(std::stoi(sm[1].str()), ent->d_name));
      }
    }
    closedir(dir);
  }

  auto numberOfBackupFiles = backupFiles.size();
  auto it = std::map<uint8_t, string>::reverse_iterator();
  if (numberOfBackupFiles < m_max_number_backup_files) {
    it = backupFiles.rbegin();
  }
  else {
    it = ++backupFiles.rbegin();
  }

  for (; it != backupFiles.rend(); it++) {
    auto origin = m_backup_log_dir + "/" + it->second;
    auto renamed = m_backup_log_dir + "/" + logFile->file_name + "." +
        std::to_string(it->first + 1);
    if (0 != std::rename(origin.c_str(), renamed.c_str())) {
      fprintf(stderr, "Logger: failed to backup %s to %s\n",
          origin.c_str(), renamed.c_str());
    }
  }

  auto origin = m_file_output_path + logFile->file_name;
  auto copied = origin + ".1";
  if (-1 == copyFile(origin, copied)) {
    fprintf(stderr, "Logger: failed to copy file %s to %s\n",
        origin.c_str(), copied.c_str());
  }
  else {
    std::remove(origin.c_str());
  }

  auto res = std::rename(copied.c_str(),
      (m_backup_log_dir + "/" + logFile->file_name + ".1").c_str());
  if (res != 0) {
    fprintf(stderr, "Logger: failed to backup file %s\n", logFile->file_name.c_str());
    return false;
  }

  return true;
}

void LoggerImpl::writeLog(std::shared_ptr<LogFile> logFile, const char* message)
{
  if (m_log_option & LogOption::Console) {
    fprintf(stdout, "%s\n", message);
  }

  if (m_log_option & LogOption::File) {
    if (validateFileSize(logFile)) {
      if (logFile->file_stream) {
        *(logFile->file_stream) << message << "\n";
        logFile->file_stream->flush();
      }
    }
  }
}

void LoggerImpl::log(level logLevel, const char* file, const char* function, int line,
    const char* format, va_list argumentList)
{
  std::unique_lock<std::mutex> l(m_log_writer_mutex);
  m_log_buffer->parseLogMessage(format, argumentList);

  if (m_is_plain_log_enable) {
    auto logfile = getPlainLogFile(logLevel, nullptr);
    if (logfile) {
      writeLog(logfile, m_log_buffer->getPlainLogMessage(logLevel, m_log_option,
          file, function, line));
    }
  }

  if (m_is_json_log_enable) {
    auto logfile = getJsonLogFile(logLevel, nullptr);
    if (logfile) {
      json j;
      j["message"] = m_log_buffer->getJsonLogMessge();
      j["level"] = m_log_buffer->logLevelToString(logLevel);
      j["component"] = "";
      j["timestamp"] = epochTimeStamp();
      writeLog(logfile, j.dump().c_str());
    }
  }
}

void LoggerImpl::log(level logLevel, const char* additional_info,
    const char* format, va_list argumentList)
{
  std::unique_lock<std::mutex> l(m_log_writer_mutex);
  m_log_buffer->parseLogMessage(format, argumentList);

  if (m_is_plain_log_enable) {
    auto logfile = getPlainLogFile(logLevel, additional_info);
    if (logfile) {
      writeLog(logfile, m_log_buffer->getPlainLogMessage(logLevel, m_log_option,
          additional_info));
    }
  }

  if (m_is_json_log_enable) {
    auto logfile = getJsonLogFile(logLevel, additional_info);
    if (logfile) {
      json j;
      j["message"] = m_log_buffer->getJsonLogMessge();
      j["level"] = m_log_buffer->logLevelToString(logLevel);
      j["component"] = additional_info;
      j["timestamp"] = epochTimeStamp();
      writeLog(logfile, j.dump().c_str());
    }
  }
}

void LoggerImpl::cleanLogFileMap()
{
  if (!m_plain_log_files.empty()) {
    for (auto& it : m_plain_log_files) {
      if (it.second->file_stream) {
        it.second->file_stream->close();
        delete it.second->file_stream;
        it.second->file_stream = nullptr;
      }
    }
    m_plain_log_files.clear();
  }

  if (!m_json_log_files.empty()) {
    for (auto& it : m_json_log_files) {
      if (it.second->file_stream) {
        it.second->file_stream->close();
        delete it.second->file_stream;
        it.second->file_stream = nullptr;
      }
    }
    m_json_log_files.clear();
  }
}

std::shared_ptr<LogFile> LoggerImpl::getPlainLogFile(level logLevel,
    const char* additional_info)
{
  if (m_separated_log && (LogLevel::Incident == logLevel)) {
    return m_plain_incident_log;
  }

  // One technical log file
  if (m_plain_log_files.size() == 1 || nullptr == additional_info) {
    return m_plain_log_files.begin()->second;
  }
  else {  // Separated technical files
    auto it = m_plain_log_files.find(additional_info);
    if (m_plain_log_files.end() != it) {
      return it->second;
    }
    else {
      return nullptr;
    }
  }
}

std::shared_ptr<LogFile> LoggerImpl::getJsonLogFile(level logLevel,
    const char* additional_info)
{
  if (m_separated_log && (LogLevel::Incident == logLevel)) {
    return m_json_incident_log;
  }

  // One technical log file
  if (m_json_log_files.size() == 1 || nullptr == additional_info) {
    return m_json_log_files.begin()->second;
  }
  else {  // Separated technical files
    auto it = m_json_log_files.find(additional_info);
    if (m_json_log_files.end() != it) {
      return it->second;
    }
    else {
      return nullptr;
    }
  }
}
