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
 * \brief  Symbol iterator example.
 * \author Vladimir Lapshin.
 */

#include <iostream>
#include <stdexcept>
#include <string>

#include "symbols.h"
#include "utf8_iterator.h"
#include "utf8_generator.h"

int main() {
  std::string line;
  while (std::getline(std::cin, line)) {
    typedef strutext::encode::Utf8Iterator<std::string::const_iterator> Utf8Iterator;
    for (Utf8Iterator it = Utf8Iterator(line.begin(), line.end()); it != Utf8Iterator(); ++it) {
      std::cout << std::hex << *it << " :";
      for (uint32_t id = 0; id < it.GetChainLen(); ++id) {
        std::cout << " " << std::hex << unsigned(it.GetChain()[id]);
      }
      std::string sym;
      strutext::encode::GetUtf8Sequence(*it, std::back_inserter(sym));
      std::cout << " : " << sym << " --> ";

      if (strutext::symbols::IsCasedLetter(*it)) {
        std::cout << "Cased Letter: ";
        if (strutext::symbols::Is<strutext::symbols::UPPERCASE_LETTER>(*it)) {
          std::string lower;
          strutext::encode::GetUtf8Sequence(strutext::symbols::ToLower(*it), std::back_inserter(lower));
          std::cout << "upper case, lower case is " << lower;
        } else if (strutext::symbols::Is<strutext::symbols::LOWERCASE_LETTER>(*it)) {
          std::string upper;
          strutext::encode::GetUtf8Sequence(strutext::symbols::ToUpper(*it), std::back_inserter(upper));
          std::cout << "lower case, upper case is " << upper;
        } else {
          std::cout << "title";
        }
      } else if (strutext::symbols::IsNumber(*it)) {
        std::cout << "number";
      } else if (strutext::symbols::IsPunctuation(*it)) {
        std::cout << "punctuation";
      } else if (strutext::symbols::IsSymbol(*it)) {
        std::cout << "symbol";
      } else if (strutext::symbols::IsSeparator(*it)) {
        std::cout << "separator";
      } else if (strutext::symbols::IsMark(*it)) {
        std::cout << "mark";
      }
      std::cout << "\n";
    }
  }

  return 0;
}

