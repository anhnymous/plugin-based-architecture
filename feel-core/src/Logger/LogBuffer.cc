#include <time.h>
#include <string.h>

#include "LogBuffer.hh"

LogBuffer::LogBuffer() : m_maximum_message_length(DEFAULT_MESSAGE_LENGTH),
                         m_context_width(DEFAULT_CONTEXT_WIDTH),
                         m_log_message_buffer(new char[m_maximum_message_length]),
                         m_temp_buffer(new char[m_maximum_message_length]),
                         m_additional_info_buffer(new char[m_context_width]),
                         m_start_time(std::chrono::high_resolution_clock::now())
{
  m_additional_info_buffer[0] = '\0';
  m_exec_time_buffer[0] = '\0';
  snprintf(m_reset_color_buffer, sizeof(m_reset_color_buffer), "%c[%dm", 0x1B, 0);
}

LogBuffer::~LogBuffer()
{
  delete[] m_log_message_buffer;
  m_log_message_buffer = NULL;
  delete[] m_temp_buffer;
  m_temp_buffer = NULL;
  delete[] m_additional_info_buffer;
  m_additional_info_buffer = NULL;
}

void LogBuffer::parseLogMessage(const char* format, va_list argumentList)
{
  m_temp_buffer[0] = '\0';
  vsnprintf(m_temp_buffer, m_maximum_message_length, format, argumentList);
}

const char* LogBuffer::getPlainLogMessage(level logLevel, option opt,
    const char* context_text)
{
  snprintf(m_additional_info_buffer, m_maximum_message_length, "%s", context_text);
  return putLogMessageTogether(logLevel, opt);
}

const char* LogBuffer::getPlainLogMessage(level logLevel, option opt, const char* file,
    const char* function, int line)
{
  snprintf(m_additional_info_buffer, m_maximum_message_length, "%s:%s:%d",
      file, function, line);
  return putLogMessageTogether(logLevel, opt);
}

const char* LogBuffer::getJsonLogMessge()
{
  return m_temp_buffer;
}

void LogBuffer::setMaxMessageLength(uint32_t length)
{
  m_maximum_message_length = length;
  delete[] m_log_message_buffer;
  delete[] m_temp_buffer;
  m_log_message_buffer = new char[m_maximum_message_length];
  m_temp_buffer = new char[m_maximum_message_length];
  m_log_message_buffer[0] = '\0';
  m_temp_buffer[0] = '\0';
}

void LogBuffer::setContextWidth(uint8_t width)
{
  m_context_width = width;
  delete[] m_additional_info_buffer;
  m_additional_info_buffer = new char[m_context_width];
}

const char* LogBuffer::logLevelToString(level logLevel)
{
  switch (logLevel)
  {
    case LogLevel::Debug:
      return "DEBU";
      break;
    case LogLevel::Info:
      return "INFO";
      break;
    case LogLevel::Warning:
      return "WARN";
      break;
    case LogLevel::Error:
      return "ERRO";
      break;
    case LogLevel::Critical:
      return "CRIT";
      break;
    case LogLevel::Incident:
      return "INCI";
      break;
    default:
      return "UNKNOWN";
  }
}

long int LogBuffer::getExecTime()
{
  return std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::high_resolution_clock::now() - m_start_time).count() / 1000000;
}

const char* LogBuffer::timeNow()
{
  auto timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  return std::ctime(&timeNow);
}

void LogBuffer::removeNewLines(char* charArray, uint32_t maxSize)
{
  for (uint32_t i = 0, skipped_chars = 0;; i++)
  {
    while (maxSize > i + skipped_chars && charArray[i + skipped_chars] != '\0' &&
        (charArray[i + skipped_chars] == '\n' ||
            charArray[i + skipped_chars] == '\r'))
    {
      skipped_chars++;
    }

    if (maxSize <= i + skipped_chars || charArray[i + skipped_chars] == '\0')
    {
      charArray[i] = '\0';
      break;
    }

    charArray[i] = charArray[i + skipped_chars];
  }
}

void LogBuffer::getTail(char* str, uint32_t maxLength)
{
  auto size = strlen(str);
  if (size > maxLength)
  {
    auto size_diff = size - maxLength;
    for (long unsigned int i = 0; i < size - size_diff; i++)
    {
      str[i] = str[i + size_diff];
    }
  }
}

int LogBuffer::getEscapeSequenceColor(level logLevel)
{
  switch (logLevel)
  {
    case LogLevel::Debug:
      return 0;
      break;
    case LogLevel::Info:
      return 32;
      break;
    case LogLevel::Warning:
      return 93;
      break;
    case LogLevel::Error:
      return 91;
      break;
    case LogLevel::Critical:
      return 31;
      break;
    default:
      return 0;
      break;
  }
}

const char* LogBuffer::putLogMessageTogether(level logLevel, option opt)
{
  if (LogOption::IncludeExecTime & opt)
  {
    snprintf(m_exec_time_buffer, sizeof(m_exec_time_buffer), " | %ld", getExecTime());
  }
  else
  {
    m_exec_time_buffer[0] = '\0';
  }

  getTail(m_additional_info_buffer, m_context_width);
  if (opt & LogOption::UseColor)
  {
    snprintf(m_log_message_buffer, m_maximum_message_length,
        "%c[%dm%25s%s | %*s | %4s | %s%s",
        0x1B, getEscapeSequenceColor(logLevel),
        timeNow(), m_exec_time_buffer, m_context_width,
        m_additional_info_buffer, logLevelToString(logLevel),
        m_temp_buffer, m_reset_color_buffer);
  }
  else
  {
    snprintf(m_log_message_buffer, m_maximum_message_length,
        "%25s%s | %*s | %4s | %s", timeNow(),
        m_exec_time_buffer, m_context_width, m_additional_info_buffer,
        logLevelToString(logLevel), m_temp_buffer);
  }

  removeNewLines(m_log_message_buffer, m_maximum_message_length);
  m_additional_info_buffer[0] = '\0';
  return m_log_message_buffer;
}
