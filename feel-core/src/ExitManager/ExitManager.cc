#include <stdio.h>
#include <stdlib.h>

#include "FCoreLogger.hh"
#include "ExitManager.hh"

template <typename E>
volatile std::atomic_bool ExitManager<E>::is_exiting{false};

template <typename E>
bool ExitManager<E>::install(E handler) noexcept
{
#ifdef VERBOSE
  fprintf(stdout, "[VERBOSE] ExitManager<E>::install() called\n");
#endif
  if (!handler) {
    LOG_ERROR("FCore: failed to install a NULL exit handler");
    return false;
  }

  int ret = atexit(handler);
  if (ret) {
    LOG_ERROR("FCore: failed to install exit handler %p", handler);
    return false;
  }

  return true;
}

template class ExitManager<ExitHandler::exit_handler_t>;
