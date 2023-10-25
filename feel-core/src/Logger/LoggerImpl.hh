#ifndef LOGGERIMPL_H_
#define LOGGERIMPL_H_

#include <mutex>
#include <memory>
#include <iostream>

#include "Logger.hh"
#include "LogBuffer.hh"

static const string BACKUP_FILE_EXTENSION = ".([[:digit:]]{1,2})$";

typedef struct LogFile {
  string file_name;
  std::ofstream* file_stream;
} LogFile;

class LoggerImpl: public Logger
{
public:
  LoggerImpl();
  ~LoggerImpl();

  void enablePlainLog(bool enable) override;
  void enableJsonLog(bool enable) override;

  bool setLogFile(string filePath, string fileName) override;
  bool setLogFile(string filePath, const TechnicalFileList& fileList,
                  string incidentFileName) override;

  void setBackupLogDir(string backupDir) override;
  void setNumberBackupFiles(const uint8_t no) override;
  void setLogFileSizeLimit(uint32_t size) override;
  void setLogLevel(level minumumLevel) override;
  void setLogOption(option op) override;
  void setLogMessageLength(uint32_t length) override;
  void setContextFieldWidth(uint8_t width) override;

  void Log(level logLevel, string additional_info, string message) override;

  void Log(level logLevel, string additional_info,
                   const char* format, ...) override;
  void Log(level logLevel, const char* additional_info,
                   const char* format, ...) override;
  void Log(level logLevel, const char* file, const char* function,
                   int line, const char* format, ...) override;
  void Log(level logLevel, const char* file, const char* function,
                   int line, string message) override;

public:
  void logTruncate(LogCategory category) override;

private:
  void logStop() override {
    std::unique_lock<std::mutex> lock(m_log_writer_mutex, std::try_to_lock);
    lock.release();

#ifdef VERBOSE
    std::cout << "[VERBOSE] logStop\n";
#endif

#if 1
    if (m_is_plain_log_enable) {
      for (auto& file : m_plain_log_files) {
        if (file.second->file_stream) {
          file.second->file_stream->flush();
          file.second->file_stream->close();
          delete file.second->file_stream;
          file.second->file_stream = nullptr;
        }
      }

      if (m_plain_incident_log) {
        m_plain_incident_log->file_stream->flush();
        m_plain_incident_log->file_stream->close();
        delete m_plain_incident_log->file_stream;
        m_plain_incident_log->file_stream = nullptr;
      }
    }

    if (m_is_json_log_enable) {
      for (auto& file : m_json_log_files) {
        if (file.second->file_stream) {
          file.second->file_stream->close();
          delete file.second->file_stream;
          file.second->file_stream = nullptr;
        }
      }

      if (m_json_incident_log) {
        m_json_incident_log->file_stream->flush();
        m_json_incident_log->file_stream->close();
        delete m_json_incident_log->file_stream;
        m_json_incident_log->file_stream = nullptr;
      }
    }
#endif

#ifdef VERBOSE
    std::cout << "[VERBOSE] End logStop\n";
#endif
  }

private:
  std::mutex m_log_writer_mutex;

  LogBuffer* m_log_buffer;
  std::map<string, std::shared_ptr<LogFile>> m_plain_log_files;
  std::map<string, std::shared_ptr<LogFile>> m_json_log_files;
  std::shared_ptr<LogFile> m_plain_incident_log;
  std::shared_ptr<LogFile> m_json_incident_log;

  option m_log_option;
  bool   m_is_plain_log_enable;
  bool   m_is_json_log_enable;
  bool   m_separated_log;

  string   m_file_output_path;
  string   m_backup_log_dir;
  uint8_t  m_max_number_backup_files;
  uint32_t m_file_size_limit;
  level    m_minimum_log_level;
  bool     m_is_log_file_set;

  bool updateOutputFileStream(std::shared_ptr<LogFile> logFile);
  bool validateFileSize(std::shared_ptr<LogFile> logFile);
  bool backupFile(std::shared_ptr<LogFile> logFile);
  void writeLog(std::shared_ptr<LogFile> logFile, const char* message);

  void log(level logLevel, const char* file, const char* function,
           int line, const char* format, va_list argumentList);
  void log(level logLevel, const char* additional_info,
           const char* format,va_list argumentList);

  void cleanLogFileMap();
  std::shared_ptr<LogFile> getPlainLogFile(level logLevel, const char* additional_info);
  std::shared_ptr<LogFile> getJsonLogFile(level logLevel, const char* additional_info);
};

#endif /* LOGGERIMPL_H_ */
