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
 * \brief  Russian morpho analyser using example.
 * \author Vladimir Lapshin.
 */

#include <fstream>
#include <stdexcept>
#include <string>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "rus_model.h"
#include "rus_alphabet.h"
#include "rus_model_description.h"
#include "morpho.h"

int main(int argc, char* argv[]) {
  try {
    namespace po = boost::program_options;

    using Morpher = strutext::morpho::Morphologist<strutext::morpho::RussianAlphabet>;
    using RussianPos = strutext::morpho::RussianPos;
    using PosSerializer = strutext::morpho::russian::PosSerializer;
    using RussianPosDecription = strutext::morpho::RussianPosDecription;

    std::string dict_path;

    po::options_description desc("Morphological analysis using example");
    desc.add_options()
      ("help", "Display options")
      ("dict", po::value<std::string>(&dict_path)->required(), "Dictionary path")
      ; // NOLINT

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 0;
    }
    po::notify(vm);

    // проверяем входные данные
    if (not boost::filesystem::exists(dict_path)) {
      throw std::invalid_argument("Dictionary file does not exist");
    }

    // инициализируем морфологический анализатор
    std::ifstream dict(dict_path.c_str());
    if (not dict.is_open()) {
      std::stringstream err;
      err << "Cannot open russian dictionary: " << dict_path;
      throw std::invalid_argument(err.str().c_str());
    }
    Morpher morpher;
    morpher.Deserialize(dict);

    // извлекаем слова и печатаем части речи
    std::string line;
    while (std::getline(std::cin, line)) {
      boost::algorithm::trim(line);
      if (not line.empty()) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
          strutext::morpho::MorphologistBase::LemList lemm_list;
          morpher.Analize(word, lemm_list);
          std::cout << word << ": {";
          unsigned idx = 0;
          for (const auto& lemma : lemm_list) {
            RussianPos::Ptr pos = PosSerializer::Deserialize(lemma.attr_);
            std::cout << RussianPosDecription::GetDecription(pos);
            if (++idx < lemm_list.size()) {
              std::cout << " | ";
            }
          }
          std::cout << "}\n";
        }
      }
    }
  } catch (std::exception &ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }
  return 0;
}

