#include <fstream>
#include <iostream>
#include <stdexcept>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "FCoreConfig.hh"
#include "ConfManager.hh"

std::shared_ptr<ConfManager> ConfManager::m_confManager{nullptr};
std::mutex                   ConfManager::m_mutex;
string                       ConfManager::m_logDir = "";
string                       ConfManager::m_logLevel = "kDefault";
uint32_t                     ConfManager::m_logSizeLimit = 1000000;

std::shared_ptr<ConfManager> ConfManager::load()
{
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_confManager) {
      try {
        m_confManager = std::shared_ptr<ConfManager>(new ConfManager);
      } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
      }
    }
  }
  return m_confManager;
}

std::shared_ptr<ConfManager> ConfManager::reload()
{
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_confManager) {
      m_confManager.reset();
      m_confManager = nullptr;
    }
  }

  return load();
}

ConfManager::ConfManager()
{
  auto confFile = string();
  if (std::getenv(CONFDIR) == NULL) {
    auto errMsg = string("FCore: environment variable CONFDIR is not set");
    throw std::runtime_error(errMsg.c_str());
  } else {
    confFile = std::getenv(CONFDIR);
    confFile += "/fcore.conf";
  }

  std::ifstream f(confFile);
  if (!f) {
    throw std::runtime_error("FCore: failed to open config file " + confFile);
  }

  nlohmann::json jsonParser;
  f >> jsonParser;
  f.close();

  try {
    if (jsonParser["fcore"]["logger"]["path"].is_string()) {
      m_logDir = jsonParser["fcore"]["logger"]["path"];
    }

    if (jsonParser["fcore"]["logger"]["level"].is_string()) {
      m_logLevel = jsonParser["fcore"]["logger"]["level"];
    }

    if (jsonParser["fcore"]["logger"]["file_size_limit"].is_number_unsigned()) {
      m_logSizeLimit = jsonParser["fcore"]["logger"]["file_size_limit"];
    }
  } catch (const nlohmann::detail::parse_error& e) {
    throw std::runtime_error(string("FCore: caught exception: ") + e.what());
  } catch (const nlohmann::detail::type_error& e) {
    throw std::runtime_error(string("FCore: caught exception: ") + e.what());
  }
}
