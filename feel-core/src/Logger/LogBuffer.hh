#ifndef LOGBUFFER_H_
#define LOGBUFFER_H_

#include <chrono>
#include <cstdio>
#include <cstdint>

#include "Logger.hh"

static constexpr uint32_t DEFAULT_MESSAGE_LENGTH = 256;
static constexpr uint8_t  DEFAULT_CONTEXT_WIDTH  = 64;

class LogBuffer
{
public:
  explicit LogBuffer();
  ~LogBuffer();

  void parseLogMessage(const char* format, va_list argumentList);
  const char* getPlainLogMessage(level logLevel, option opt, const char* context_text);
  const char* getPlainLogMessage(level logLevel, option opt, const char* file,
                                 const char* function, int line);
  const char* getJsonLogMessge();
  void setMaxMessageLength(uint32_t length);
  void setContextWidth(uint8_t width);
  const char* logLevelToString(level logLevel);

private:
  uint32_t m_maximum_message_length;
  uint8_t  m_context_width;

  char* m_log_message_buffer;
  char* m_temp_buffer;
  char* m_additional_info_buffer;
  char  m_exec_time_buffer[20];
  char  m_reset_color_buffer[5];
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;

  long int getExecTime();
  const char* timeNow();
  void removeNewLines(char* charArray, uint32_t maxSize);
  void getTail(char* str, uint32_t maxLength);
  int getEscapeSequenceColor(level logLevel);
  const char* putLogMessageTogether(level logLevel, option opt);
};

#endif /* LOGBUFFER_H_ */
