#include <stdio.h>

#include "FCoreLogger.hh"
#include "ExitHandlers.hh"

void ExitHandler::stopLogger(void)
{
#ifdef VERBOSE2
  fprintf(stdout, "exitHandler stopLogger(void) called\n");
#endif
  LOG.logStop();
}
