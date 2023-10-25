#include <unistd.h>

#include <string>
#include <thread>
#include <iostream>
#include <algorithm>

#include "Interfaces.hh"
#include "MainHandler.hh"
#include "FFeatureConfig.hh"

bool MainHandler::isMatchedArguments(int argc, char** argv __attribute__((unused)))
{
  if (argc == 1) return true;
  if (argc == 3) {
    std::string option = argv[1];
    std::string value  = argv[2];
    if (option == "-f" || option == "--feature") {
      if (!value.empty()) {
        m_feature_name = value;
        return true;
      }
      return false;
    }
    return false;
  }
  return false;
}

AppHandler::RetCode MainHandler::start(int argc __attribute__((unused)),
                                       char** argv __attribute__((unused)))
{
  std::cout << "Runtime Containerized Application/Feature ..." << std::endl;

  bool run_x = false;
  bool run_y = false;

  if (!m_feature_name.compare("x") || !m_feature_name.compare("X")) {
    std::cout << "== INFO: starting feature X" << std::endl;
    run_x = true;
  } else if (!m_feature_name.compare("y") || !m_feature_name.compare("Y")) {
    std::cout << "== INFO: starting feature Y" << std::endl;
    run_y = true;
  } else if (!m_feature_name.compare("xy") || !m_feature_name.compare("XY")) {
    std::cout << "== INFO: starting feature X and Y" << std::endl;
    run_x = true;
    run_y = true;
  } else {
    std::cerr<< "== ERROR: Missing feature!" << std::endl;
    return AppHandler::RetCode::INIT_ERROR;
  }

  auto fx = []() -> void {
    auto x_read = Interfaces::xxx_read();
    fprintf(stdout, "== INFO: Interfaces::xxx_read() = %d\n", x_read);
    auto x_write = Interfaces::xxx_write(x_read + 1);
    if (x_write) {
      fprintf(stdout, "== INFO: Interfaces::xxx_write() TRUE\n");
    } else {
      fprintf(stdout, "== INFO: Interfaces::xxx_write() FALSE\n");
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  };

  auto fy = []() -> void {
    auto y_read = Interfaces::yyy_read();
    std::cout << "== INFO: Interfaces::yyy_read() = " << y_read << std::endl;
    auto y_write = Interfaces::yyy_write(y_read + 1.0f);
    if (y_write) {
      fprintf(stdout, "== INFO: Interfaces::yyy_write() TRUE\n");
    } else {
      fprintf(stdout, "== INFO: Interfaces::yyy_write() FALSE\n");
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  };

  int16_t rounds = 1000;
  try {
    while (rounds-- > 0) {
      if (run_x) fx();
      if (run_y) fy();
    }
    std::cout << "== INFO: Feel Feature shutdowns" << std::endl;
  } catch (const std::system_error& e) {
    std::cout << "== INFO: Feel Feature caught system error" << std::endl;
    return AppHandler::RetCode::INIT_ERROR;
  } catch (const std::runtime_error& e) {
    std::cout << "== INFO: Feel Feature caught runtime error" << std::endl;
    return AppHandler::RetCode::INIT_ERROR;
  }

  return AppHandler::RetCode::INIT_OK;
}
