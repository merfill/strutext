/** Copyright &copy; 2013, Vladimir Lapshin.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 * \file   ut_main.cpp
 * \brief  Unit test main function.
 * \author Vladimir Lapshin.
 * \date   09.10.2013
 */

#include <iostream>
#include <stdexcept>
#include <regex>

#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/regex.hpp>

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/configurator.h>

namespace {

// Boost test initialization function.
boost::unit_test::test_suite* init_unit_test_suite(int argc, char* argv[]) {
  if (not std::string(UNIT_TEST_MODULE_NAME).empty()) {
    boost::unit_test::framework::master_test_suite().p_name.value = UNIT_TEST_MODULE_NAME;
  }
  return nullptr;
}

// Инициализация логера.
void InitLogger(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    boost::cmatch cm;
    if (boost::regex_match(argv[i], cm, boost::regex("--log_props=([^ ]+)", boost::format_sed))) {
      // Initialize logger by data from the file.
      std::cerr << "Read Log4Cplus property file: \"" << cm[1] << "\"...\n";
      log4cplus::PropertyConfigurator::doConfigure(cm[1]);
      break;
    }
  }

  // By default set ERROR level on root logger.
  log4cplus::Logger root_logger = log4cplus::Logger::getRoot();
  root_logger.setLogLevel(log4cplus::ERROR_LOG_LEVEL);
}

} // namespace.

int main(int argc, char* argv[]) {
  try {
    InitLogger(argc, argv);
    return boost::unit_test::unit_test_main(&init_unit_test_suite, argc, argv);
  } catch (const std::exception& err) {
    std::cerr << err.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Unknown error\n";
    return 2;
  }

  return 0;
}
