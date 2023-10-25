#include <unistd.h>

#include <string>
#include <thread>
#include <iostream>
#include <algorithm>

#include "GIID_DB.hh"
#include "grpcServer.hh"
#include "FCoreLogger.hh"
#include "FCoreConfig.hh"
#include "MainHandler.hh"
#include "ConfManager.hh"
#include "ExitManager.hh"
#include "SignalManager.hh"

bool MainHandler::isMatchedArguments(int argc, char** argv __attribute__((unused)))
{
  if (argc == 1) return true;
  return false;
}

AppHandler::RetCode MainHandler::start(int argc __attribute__((unused)),
                                       char** argv __attribute__((unused)))
{
  std::cout << "FeelCore (gRPC server)" << std::endl;

  auto conf = ConfManager::load();
  if (!conf) {
    std::cerr << "FCore: cannot load configuration, exit!" << std::endl;
    return AppHandler::RetCode::INIT_ERROR;
  }

  if (!FCoreLoggerInit(conf)) {
    std::cerr << "FCore: failed to init FCore logger, exit!" << std::endl;
    return AppHandler::RetCode::INIT_ERROR;
  }

  if (SignalManager<SIGNAL_handler>::install(SIGTERM, SIGTERM_handler)) {
    LOG_INFO("FCore: installed signal handler for SIGTERM");
  } else {
    std::cerr << "FCore: failed to install signal handler for SIGINT, exit\n";
    return AppHandler::RetCode::INIT_ERROR;
  }

  if (ExitManager<ExitHandler::exit_handler_t>::install(ExitHandler::stopLogger)) {
    LOG_INFO("FCore: installed exit handler `stopLogger`");
  } else {
    std::cerr << "FCore: failed to install exit handler `stopLogger`\n";
    return AppHandler::RetCode::INIT_ERROR;
  }

  LOG_INFO("FCore: build GIID database ...");
  if (!GIID_DB::getInstance().build_db()) {
    LOG_ERROR("FCore: failed to build GIID database");
    return AppHandler::RetCode::INIT_ERROR;
  }
  LOG_INFO("FCore: build GIID database completed");

  try {
    grpcServer server;
    server.set_ipv4(FCore_DEFAULT_HOST).set_port(FCore_DEFAULT_PORT);
    auto server_f = [&server]() -> void { server.start(); };
    LOG_INFO("FCore: starting FCore server ...");
    std::thread server_thread(server_f);

    while (!ExitManager<ExitHandler::exit_handler_t>::is_exiting) { sleep (1); }

    LOG_INFO("FCore: received shutdown order, doing safe shutdown ...");
    if (server.is_running()) { server.stop(); }
    server_thread.join();

    LOG_INFO("FCore: stopped FCore server, shutdown safely");
  } catch (const std::system_error& e) {
    LOG_ERROR("FCore: caught system error: %s", e.what());
    return AppHandler::RetCode::INIT_ERROR;
  } catch (const std::runtime_error& e) {
    LOG_ERROR("FCore: caught runtime error: %s", e.what());
    return AppHandler::RetCode::INIT_ERROR;
  }

  return AppHandler::RetCode::INIT_OK;
}
