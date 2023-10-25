#include <string>
#include <iostream>

#include "HelpHandler.hh"
#include "FFeatureConfig.hh"

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
  std::cout << "Feel Feature" << std::endl;
  std::cout << "Usage:" << std::endl;
  std::cout << " Start service: ./feel_feature [-f/--feature <name>]\n" << std::endl;
  std::cout << " Show version: ./feel_feature -v; ./feel_feature --version" << std::endl;
  std::cout << " Show help: ./feel_feature -h; ./feel_feature --help" << std::endl;
  return RetCode::INIT_OK;
}







