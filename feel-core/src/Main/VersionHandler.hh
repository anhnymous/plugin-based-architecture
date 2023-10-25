#ifndef VERSIONHANDLER_H_
#define VERSIONHANDLER_H_

#include "AppHandler.hh"

class VersionHandler: public AppHandler
{
public:
  VersionHandler(VersionHandler&&) = delete;
  VersionHandler(const VersionHandler&) = delete;
  VersionHandler& operator=(VersionHandler&&) = delete;
  VersionHandler& operator=(const VersionHandler&) = delete;

  explicit VersionHandler(void) = default;
  virtual ~VersionHandler() = default;

  bool isMatchedArguments(int argc, char** argv) override final;
  RetCode start(int argc, char** argv) override final;
};

#endif /* VERSIONHANDLER_H_ */
