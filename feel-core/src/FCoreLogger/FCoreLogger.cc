#include "FCoreLogger.hh"

bool FCoreLoggerInit(const std::shared_ptr<ConfManager> conf)
{
  LOG.enablePlainLog(true);
  LOG.enableJsonLog(false);
  LOG.setLogOption(LogOption::File);
  TechnicalFileList flist{{"technical", "technical.log"}};
  if (!LOG.setLogFile(conf->logDir(), flist, "incident.log")) return false;
  LOG.setBackupLogDir(conf->logDir());
  LOG.setNumberBackupFiles(5);
  LOG.setLogFileSizeLimit(conf->logSizeLimit());
  LOG.setContextFieldWidth(LOG_CONTEXT_WIDTH);
  LOG.setLogMessageLength(LOG_MESSAGE_LENGTH);
  LOG.setLogLevel(getLogLevel(conf->logLevel()));
  return true;
}

level getLogLevel(const string& level)
{
  if (!level.compare("kDebug")) return LogLevel::Debug;
  if (!level.compare("kInfo")) return LogLevel::Info;
  if (!level.compare("kWarning")) return LogLevel::Warning;
  if (!level.compare("kError")) return LogLevel::Error;
  if (!level.compare("kCritical")) return LogLevel::Critical;
  if (!level.compare("kIncident")) return LogLevel::Incident;
  return LogLevel::Debug;
}
