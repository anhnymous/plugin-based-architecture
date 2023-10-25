#include <stdio.h>

#include "ExitManager.hh"
#include "SignalHandlers.hh"

void SIGTERM_handler(int sig __attribute__((unused)),
                     siginfo_t* sigInfo __attribute__((unused)),
                     void* uc __attribute__((unused)))
{
#ifdef VERBOSE
  fprintf(stdout, "[VERBOSE] SIGTERM_handler called, set is_exiting to TRUE\n");
#endif
  ExitManager<ExitHandler::exit_handler_t>::is_exiting = true;
}
