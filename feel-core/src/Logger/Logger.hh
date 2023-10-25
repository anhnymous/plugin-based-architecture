#ifndef LOGGER_H_
#define LOGGER_H_

#include <map>
#include <string>
#include <cstdint>
#include <string.h>

#include "LogCommonDefs.hh"

using string = std::string;

#define LOG Logger::getLogger()
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define VA_ARGS(...),##__VA_ARGS__
#define LOG_LEVEL(b, a, ...) Logger::getLogger().Log(b, __FILENAME__, __func__, \
                             __LINE__, a VA_ARGS(__VA_ARGS__))

#define LOG_DEBUG(a, ...) LOG_LEVEL(LogLevel::Debug, a VA_ARGS(__VA_ARGS__))
#define LOG_INFO(a, ...) LOG_LEVEL(LogLevel::Info, a VA_ARGS(__VA_ARGS__))
#define LOG_WARNING(a, ...) LOG_LEVEL(LogLevel::Warning, a VA_ARGS(__VA_ARGS__))
#define LOG_ERROR(a, ...) LOG_LEVEL(LogLevel::Error, a VA_ARGS(__VA_ARGS__))
#define LOG_CRITICAL(a, ...) LOG_LEVEL(LogLevel::Critical, a VA_ARGS(__VA_ARGS__))
#define LOG_INCIDENT(a, ...) LOG_LEVEL(LogLevel::Incident, a VA_ARGS(__VA_ARGS__))

using technical_key = string;
using technical_filename = string;
typedef std::map<technical_key, technical_filename> TechnicalFileList;

class Logger
{
public:
  explicit Logger() = default;
  virtual ~Logger() = default;
  static Logger& getLogger();

  virtual void enablePlainLog(bool enable) = 0;
  virtual void enableJsonLog(bool enable) = 0;

  virtual bool setLogFile(string filePath, string fileName) = 0;
  virtual bool setLogFile(string filePath, const TechnicalFileList& fileList,
                          string incidentFileName) = 0;

  virtual void setBackupLogDir(string backupDir) = 0;
  virtual void setNumberBackupFiles(const uint8_t no) = 0;
  virtual void setLogFileSizeLimit(uint32_t size) = 0;
  virtual void setLogLevel(level minumumLevel) = 0;
  virtual void setLogOption(option op) = 0;
  virtual void setLogMessageLength(uint32_t length) = 0;
  virtual void setContextFieldWidth(uint8_t width) = 0;

  virtual void Log(level logLevel, string additional_info, string message) = 0;

  virtual void Log(level logLevel, string additional_info,
                   const char* format, ...) = 0;
  virtual void Log(level logLevel, const char* additional_info,
                   const char* format, ...) = 0;
  virtual void Log(level logLevel, const char* file, const char* function,
                   int line, const char* format, ...) = 0;
  virtual void Log(level logLevel, const char* file, const char* function,
                   int line, string message) = 0;
public:
  virtual void logTruncate(LogCategory) = 0;

private:
  friend class ExitHandler;
  virtual void logStop() = 0;
};

#endif /* LOGGER_H_ */
