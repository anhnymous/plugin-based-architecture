#ifndef SIGNALHANDLERS_H_
#define SIGNALHANDLERS_H_

#include <signal.h>

using SIGNAL_handler = void (*)(int, siginfo_t*, void*);
void SIGTERM_handler(int sig, siginfo_t* sigInfo, void* uc);

#endif /* SIGNALHANDLERS_H_ */
