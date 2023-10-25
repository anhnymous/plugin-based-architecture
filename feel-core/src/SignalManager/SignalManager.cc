#include <errno.h>
#include <signal.h>

#include "FCoreLogger.hh"
#include "SignalManager.hh"

template <typename H>
bool SignalManager<H>::install(int signum, H handler)
{
  if (signum < 1 || signum >= NSIG) {
    LOG_ERROR("FCore: install an invalid signal (number %d)", signum);
    return false;
  }

  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = handler;
  sigemptyset(&sa.sa_mask);
  if (-1 == sigaction(signum, &sa, NULL)) {
    LOG_ERROR("FCore: can not install signal hander for signal (number %d)", signum);
    return false;
  }

  return true;
}

template <typename H>
bool SignalManager<H>::ignore(int signum)
{
  if (SIG_ERR == signal(signum, SIG_IGN)) {
    LOG_ERROR("FCore: can not ignore signal (number %d)", signum);
    return false;
  }

  return true;
}

template class SignalManager<SIGNAL_handler>;
