#ifndef HELPHANDLER_H_
#define HELPHANDLER_H_

#include "AppHandler.hh"

class HelpHandler: public AppHandler
{
public:
  HelpHandler(HelpHandler&&) = delete;
  HelpHandler(const HelpHandler&) = delete;
  HelpHandler& operator=(HelpHandler&&) = delete;
  HelpHandler& operator=(const HelpHandler&) = delete;

  explicit HelpHandler(void) = default;
  virtual ~HelpHandler() = default;

  bool isMatchedArguments(int argc, char** argv) override final;
  RetCode start(int argc, char** argv) override final;
};

#endif /* HELPHANDLER_H_ */
