#ifndef FCORELOGGER_H_
#define FCORELOGGER_H_

#include <string>
#include <memory>
#include <cstdint>

#include "Logger.hh"
#include "ConfManager.hh"

using string = std::string;

constexpr uint32_t LOG_FILE_SIZE      = 4 * 1024;   // bytes
constexpr uint32_t LOG_CONTEXT_WIDTH  = 4 * 8;      // bytes
constexpr uint32_t LOG_MESSAGE_LENGTH = 4 * 128;    // bytes

bool FCoreLoggerInit(const std::shared_ptr<ConfManager> conf);
level getLogLevel(const string& level);

#endif /* FCORELOGGER_H_ */
