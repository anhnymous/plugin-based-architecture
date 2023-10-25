#include <string>
#include <iostream>

#include "VersionHandler.hh"
#include "FFeatureVersion.hh"

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
  std::cout << "Feal Feature" << std::endl;
  std::cout << "Version " << FFeature_VERSION_MAJOR << "." <<
                             FFeature_VERSION_MINOR << "." <<
                             FFeature_VERSION_PATCH << std::endl;

  return RetCode::INIT_OK;
}




