#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "Application.hh"
#include "HelpHandler.hh"
#include "MainHandler.hh"
#include "VersionHandler.hh"

int main(int argc, char** argv)
{
  Application app(argc, argv);
  app.useHandler(std::make_shared<HelpHandler>());
  app.useHandler(std::make_shared<MainHandler>());
  app.useHandler(std::make_shared<VersionHandler>());
  app.useFallbackHandler(std::make_shared<HelpHandler>());
  auto ret = app.start();
  return ((int)ret);
}
