#ifndef EXITMANAGER_H_
#define EXITMANAGER_H_

#include <atomic>
#include <signal.h>

#include "ExitHandlers.hh"

template <typename E>
class ExitManager
{
public:
  explicit ExitManager() = delete;
  ~ExitManager() = default;

  ExitManager(ExitManager&&) = delete;
  ExitManager(const ExitManager&) = delete;
  ExitManager& operator=(ExitManager&&) = delete;
  ExitManager& operator=(const ExitManager&) = delete;

  static bool install(E handler) noexcept;

private:
  friend class MainHandler;
  friend void SIGTERM_handler(int sig, siginfo_t* sigInfo, void* uc);
  static volatile std::atomic_bool is_exiting;
};

#endif /* EXITMANAGER_H_ */
