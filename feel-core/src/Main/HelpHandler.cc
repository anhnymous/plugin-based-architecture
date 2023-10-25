#include <string>
#include <iostream>

#include "FCoreConfig.hh"
#include "HelpHandler.hh"

bool HelpHandler::isMatchedArguments(int argc, char** argv)
{
  if (argc == 2) {
      std::string option = argv[1];
    if (option == "-h" || option == "--help") {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

AppHandler::RetCode HelpHandler::start(int argc __attribute__((unused)),
                                       char** argv __attribute__((unused)))
{
  std::cout << "Feel Core" << std::endl;
  std::cout << "Usage:" << std::endl;
  std::cout << " Start: ./feel_core\n" << std::endl;
  std::cout << " Show version: feel_core -v; feel_core --version" << std::endl;
  std::cout << " Show help: feel_core -h; feel_core --help" << std::endl;
  return RetCode::INIT_OK;
}
