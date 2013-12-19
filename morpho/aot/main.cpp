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
 * \brief  AOT parser implementation.
 * \author Vladimir Lapshin.
 */

#include <stdint.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <stdexcept>
#include <iterator>

#include <boost/program_options.hpp>
#include <boost/make_shared.hpp>

#include "utf8_iterator.h"
#include "utf8_generator.h"
#include "symbols.h"
#include "morpho_modifier.h"
#include "morpho.h"
#include "aot_parser.h"
#include "eng_aot_parser.h"
#include "rus_aot_parser.h"

namespace {

typedef std::map<std::string, uint32_t> Tabs;
typedef std::map<uint32_t, std::string> MainFormSuffixList;

MainFormSuffixList main_form_suffix_list;


void ParseTabFile(strutext::morpho::AotParser::Ptr parser, const std::string& fname, const std::string& model, Tabs& tabs) {
  // Open tab file.
  std::ifstream tab_file(fname.c_str());
  if (not tab_file.is_open()) {
    throw std::invalid_argument(("cannot open tab file: \"" + fname + "\"").c_str());
  }

  // Go through strings of tab file.
  std::string line;
  while (not tab_file.eof()) {
    std::getline(tab_file, line);
    // Skip empty strings
    if (line.length() == 0) {
      continue;
    }

    // Clear string from caret symbol.
    boost::algorithm::erase_all(line, "\r");
    // Drop empty line.
    if (line[0] == '\n') {
      continue;
    }
    // Drop comments.
    if (line.substr(0, 2) == "//") {
      continue;
    }

    // Drop spaces on the line head.
    std::string tmp;
    size_t pos = line.find_first_not_of(" ");
    if (pos == std::string::npos) {
      continue;
    } else if (pos and pos < std::string::npos) {
      line = line.substr(pos);
    }
    if (line.length() == 0) {
      continue;
    }
    std::string id_str;
    uint32_t attrs = parser->ParseAttrLine(line, id_str);
    tabs[id_str] = attrs;
  }
}

template<class Alphabet>
void ReadSuffixSection(strutext::morpho::AotParser::Ptr parser, std::ifstream& mfile,
                        strutext::morpho::Morphologist<Alphabet>& morpher, const Tabs& tabs) {
  std::cerr << "Starting read suffix section...\n";

  // Firstly, read number of lines to read.
  std::string line;
  std::getline(mfile, line);
  boost::algorithm::erase_all(line, "\r");
  if (line.length() == 0) {
    throw std::invalid_argument("It must be number of lines to read in the section begin");
  }
  unsigned line_count = atoi(line.c_str());
  if (line_count == 0) {
    throw std::invalid_argument("Number of lines in a section must be more then 0");
  }

  std::cerr << line_count << " lines in the section\n";

  // Then read number of lines.
  for (unsigned i = 0; i < line_count; ++i) {
    if (mfile.eof()) {
      throw std::invalid_argument("Unexpected end of file while read section");
    }
    std::getline(mfile, line);

    // Split line to fields.
    std::vector<std::string> fields;
    boost::algorithm::split(fields, line, boost::is_any_of("%"));

    // Add suffix line.
    uint32_t line_id = strutext::morpho::MorphoModifier::AddSuffixLine(morpher);

    // Go throuch the fields.
    for (unsigned i = 1; i < fields.size(); ++i) { // Drop first field.
      // Split field to suffix and tab.
      std::vector<std::string> pair;
      boost::algorithm::split(pair, fields[i], boost::is_any_of("*"));
      if (pair.size() < 2) {
        std::stringstream st;
        st << "Incorrect format of suffix section on line #"
           << line_id << " in tab \"" << fields[i] << "\"";
        throw std::invalid_argument(st.str().c_str());
      }
      boost::algorithm::erase_all(pair[1], "\r");

      // Search in tabs file to get morphological attributes.
      Tabs::const_iterator it = tabs.find(pair[1]);
      if (it == tabs.end()) {
        std::stringstream st;
        st << "Incorrect tab (" << pair[0] << ";" << pair[1] << ") in suffix section on line #" << line_id;
        throw std::invalid_argument(st.str().c_str());
      } else if (it->second) {
        // Normilize string to lower register.
        typedef strutext::encode::Utf8Iterator<std::string::iterator> Utf8Iterator;
        std::vector<uint32_t> norm_suffix;
        for (Utf8Iterator sym_it(pair[0].begin(), pair[0].end()); sym_it != Utf8Iterator(); ++sym_it) {
          norm_suffix.push_back(strutext::symbols::ToLower(*sym_it));
        }
         // Add suffix.
        strutext::morpho::MorphoModifier::AddSuffix(morpher, line_id, it->second, norm_suffix.begin(), norm_suffix.end());

        // Remember main form suffix for this line.
        if (parser->IsMainFormCode(pair[1])) {
          std::string suffix;
          strutext::encode::GetUtf8Sequence(norm_suffix.begin(), norm_suffix.end(), std::back_inserter(suffix));
          main_form_suffix_list.insert(std::make_pair(line_id, suffix));
       }
      }
    }
  }

  std::cerr << "End of reading suffix table...\n";
}

void DropSection(std::ifstream& mfile) {
  std::cerr << "Start reading droping section...\n";
  // Firstly, read number of lines to drop.
  std::string line;
  std::getline(mfile, line);
  boost::algorithm::erase_all(line, "\r");
  if (line.length() == 0 or line[0] == '\n') {
    throw std::invalid_argument("It must be number of lines to read in the section begin");
  }
  unsigned line_count = atoi(line.c_str());
  std::cerr << line_count << " lines in the section\n";

  // Then read number of lines.
  for (unsigned i = 0; i < line_count; ++i) {
    if (mfile.eof()) {
      throw std::invalid_argument("Unexpected end of file while read section");
    }
    std::getline(mfile, line);
  }

  std::cerr << "End of reading the section\n";
}

template<class Alphabet>
void ReadDictionarySection(std::ifstream& mfile, strutext::morpho::Morphologist<Alphabet>& morpher) {
  std::cerr << "Start reading dictionaty section...\n";

  // Firstly, read number of lines to drop.
  std::string line;
  std::getline(mfile, line);
  boost::algorithm::erase_all(line, "\r");
  if (line.length() == 0 or line[0] == '\n') {
    throw std::invalid_argument("It must be number of lines to read in the section begin");
  }
  unsigned line_count = atoi(line.c_str());
  if (line_count == 0) {
    throw std::invalid_argument("Number of lines in a section must be more then 0");
  }

  std::cerr << line_count << " lines in the section\n";

  // Then read number of lines.
  for (unsigned i = 0; i < line_count; ++i) {
    if (mfile.eof()) {
      throw std::invalid_argument("Unexpected end of file while read section");
    }
    std::getline(mfile, line);
    boost::algorithm::erase_all(line, "\r");
    if (line.length() == 0 or line[0] == '\n') {
      continue;
    }

    // Split line.
    std::vector<std::string> fields;
    boost::algorithm::split(fields, line, boost::is_any_of(" "));
    if (fields.size() < 2) {
      std::stringstream st;
      st << "Incorrect word definition (" << line << "\"";
      throw std::invalid_argument(st.str().c_str());
    }
    // Go through line id list...
    unsigned line_id = atoi(fields[1].c_str());
    uint32_t lem_id = i + 1;

    // Normilize string to lower register.
    typedef strutext::encode::Utf8Iterator<std::string::iterator> Utf8Iterator;
    std::vector<uint32_t> norm_base;
    for (Utf8Iterator sym_it(fields[0].begin(), fields[0].end()); sym_it != Utf8Iterator(); ++sym_it) {
      norm_base.push_back(strutext::symbols::ToLower(*sym_it));
    }

    // Calculate main form.
    std::string main_form;
    strutext::encode::GetUtf8Sequence(norm_base.begin(), norm_base.end(), std::back_inserter(main_form));
    main_form += main_form_suffix_list[line_id];

    strutext::morpho::MorphoModifier::AddBase(morpher, lem_id, line_id, norm_base.begin(), norm_base.end(), main_form);
  }

  std::cerr << "End of reading the section\n";
}

template<class Alphabet>
void ReadDictFile(strutext::morpho::AotParser::Ptr parser, const std::string& dname, const std::string& bname, const Tabs& tabs) {
  std::cerr << "Starting parse dictionary file...\n";
  strutext::morpho::Morphologist<Alphabet> morpher;

  // Open dictionaty file.
  std::ifstream dic_file(dname.c_str());
  if (not dic_file.is_open()) {
    throw std::invalid_argument(("cannot open dictionary file: \"" + dname + "\"").c_str());
  }
  ReadSuffixSection(parser, dic_file, morpher, tabs);
  DropSection(dic_file);
  DropSection(dic_file);
  DropSection(dic_file);
  ReadDictionarySection(dic_file, morpher);

  // Serialize morphologist object.
  std::cerr << "Start serialization...\n";
  std::ofstream bfile(bname.c_str());
  if (not bfile.is_open()) {
    throw std::invalid_argument(("cannot open binary dictionary file: \"" + bname + "\"").c_str());
  }
  morpher.Serialize(bfile);
  std::cerr << "Serialization completed\n";

  std::cerr << "End of reading dictionary file\n";
}

} // namespace.

int main(int argc, char* argv[]) {
  try {
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help")
      ("tab,t", po::value<std::string>(), "tab file name")
      ("dict,d", po::value<std::string>(), "dictionary file name")
      ("bin,b", po::value<std::string>(), "binary dictionary file name")
      ("model,m", po::value<std::string>(), "language model: eng, rus")
      ("verbose,v", "produce process info to stderr");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cerr << desc << "\n";
      return 0;
    }

    std::string tab_file_name;
    if (vm.count("tab")) {
      tab_file_name = vm["tab"].as<std::string>();
    } else {
      std::cerr << desc << "\n";
      return 1;
    }

    std::string dict_file_name;
    if (vm.count("dict")) {
      dict_file_name = vm["dict"].as<std::string>();
    } else {
      std::cerr << desc << "\n";
      return 1;
    }

    std::string bin_file_name;
    if (vm.count("bin")) {
      bin_file_name = vm["bin"].as<std::string>();
    } else {
      std::cerr << desc << "\n";
      return 1;
    }

    std::string model;
    if (vm.count("model")) {
      model = vm["model"].as<std::string>();
    } else {
      std::cerr << desc << "\n";
      return 1;
    }

    strutext::morpho::AotParser::Ptr parser;
    if (model == "rus") {
      parser = boost::make_shared<strutext::morpho::RussianAotParser>();
    } else if (model == "eng") {
      parser = boost::make_shared<strutext::morpho::EnglishAotParser>();
    } else {
      std::cerr << "incorrect language model name: \"" << model << "\"\n";
      return 1;
    }

    // Parse tab file.
    std::cerr << "Starting parse tab file...\n";
    Tabs tabs;
    ParseTabFile(parser, tab_file_name, model, tabs);
    std::cerr << "Parsing tab file completed, " << tabs.size() << " tabs extracted\n";

    if (model == "rus") {
      ReadDictFile<strutext::morpho::RussianAlphabet>(parser, dict_file_name, bin_file_name, tabs);
    } else if (model == "eng") {
      ReadDictFile<strutext::morpho::EnglishAlphabet>(parser, dict_file_name, bin_file_name, tabs);
    }
  } catch (const std::exception& err) {
    std::cerr << err.what() << "\n";
    return 2;
  }

  return 0;
}
