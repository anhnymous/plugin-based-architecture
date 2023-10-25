#ifndef SIGNALMANAGER_H_
#define SIGNALMANAGER_H_

#include "SignalHandlers.hh"

template <typename H>
class SignalManager
{
public:
  explicit SignalManager() = delete;
  ~SignalManager() = default;
  static bool install(int signum, H handler);
  static bool ignore(int signum);
};

#endif /* SIGNALMANAGER_H_ */
