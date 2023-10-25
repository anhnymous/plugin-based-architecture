#ifndef MAINHANDLER_H_
#define MAINHANDLER_H_

#include <cstdint>

#include "AppHandler.hh"

class MainHandler: public AppHandler
{
public:
  MainHandler(MainHandler&&) = delete;
  MainHandler(const MainHandler&) = delete;
  MainHandler& operator=(MainHandler&&) = delete;
  MainHandler& operator=(const MainHandler&) = delete;

  explicit MainHandler(void) = default;
  virtual ~MainHandler() = default;

  bool isMatchedArguments(int argc, char** argv) override final;
  RetCode start(int argc, char** argv) override final;

private:
  std::string m_feature_name;
};

#endif /* MAINHANDLER_H_ */
