#include <string>
#include <iostream>

#include "FCoreVersion.hh"
#include "VersionHandler.hh"

bool VersionHandler::isMatchedArguments(int argc, char** argv)
{
  if (argc == 2) {
      std::string option = argv[1];
    if (option == "-v" || option == "--version") {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

AppHandler::RetCode VersionHandler::start(int argc __attribute__((unused)),
                                          char** argv __attribute__((unused)))
{
  std::string option = argv[1];
  std::cout << "Feel Core (gRPC server)" << std::endl;
  std::cout << "Version " << FCore_VERSION_MAJOR << "." <<
                             FCore_VERSION_MINOR << "." <<
                             FCore_VERSION_PATCH << std::endl;

  return RetCode::INIT_OK;
}
