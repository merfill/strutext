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
 * \file   test.cpp
 * \brief  Ngram iterator unit test.
 * \author Vladimir Lapshin.
 * \date   23.11.2013
 */

#include <stdint.h>

#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

#include "utf8_iterator.h"
#include "utf8_generator.h"
#include "ngram_iterator.h"
#include "symbol_iterator.h"
#include "word_iterator.h"

namespace strutext { namespace utility {

BOOST_AUTO_TEST_CASE(Utility_NgrammIterator_Symbols) {
  std::string text = "abcde";
  typedef NgramIterator<char, std::string::const_iterator> NgramIteratorImpl;
  std::vector<std::string> result;
  for (NgramIteratorImpl it(text.begin(), text.end(), 1, 5), end; it != end; ++it) {
    NgramIteratorImpl::Ngram ngram = *it;
    result.push_back(std::string(ngram.begin(), ngram.end()));
  }
  BOOST_REQUIRE(result.size() == 15);
  BOOST_CHECK(result[0] == "a");
  BOOST_CHECK(result[1] == "ab");
  BOOST_CHECK(result[2] == "abc");
  BOOST_CHECK(result[3] == "abcd");
  BOOST_CHECK(result[4] == "abcde");
  BOOST_CHECK(result[5] == "b");
  BOOST_CHECK(result[6] == "bc");
  BOOST_CHECK(result[7] == "bcd");
  BOOST_CHECK(result[8] == "bcde");
  BOOST_CHECK(result[9] == "c");
  BOOST_CHECK(result[10] == "cd");
  BOOST_CHECK(result[11] == "cde");
  BOOST_CHECK(result[12] == "d");
  BOOST_CHECK(result[13] == "de");
  BOOST_CHECK(result[14] == "e");
}

BOOST_AUTO_TEST_CASE(Utility_NgrammIterator_Utf8RussianText) {
  std::string text = "абвгдеж";
  typedef encode::Utf8Iterator<std::string::const_iterator> Utf8IteratorImpl;
  typedef NgramIterator<symbols::SymbolCode, Utf8IteratorImpl> NgramIteratorImpl;
  std::vector<std::string> result;
  Utf8IteratorImpl utf8_begin(text.begin(), text.end()), utf8_end;
  for (NgramIteratorImpl it(utf8_begin, utf8_end, 2, 3), end; it != end; ++it) {
    NgramIteratorImpl::Ngram ngram = *it;
    std::string text;
    encode::GetUtf8Sequence(ngram.begin(), ngram.end(), std::back_inserter(text));
    result.push_back(text);
  }
  BOOST_REQUIRE(result.size() == 11);
  BOOST_CHECK(result[0] == "аб");
  BOOST_CHECK(result[1] == "абв");
  BOOST_CHECK(result[2] == "бв");
  BOOST_CHECK(result[3] == "бвг");
  BOOST_CHECK(result[4] == "вг");
  BOOST_CHECK(result[5] == "вгд");
  BOOST_CHECK(result[6] == "гд");
  BOOST_CHECK(result[7] == "где");
  BOOST_CHECK(result[8] == "де");
  BOOST_CHECK(result[9] == "деж");
  BOOST_CHECK(result[10] == "еж");
}

BOOST_AUTO_TEST_CASE(Utility_SymbolAlphaIterator_EnglishGeneral) {
  typedef FilterIterator<symbols::SymbolCode, std::string::const_iterator, AlphaFilter, SymTransform> IteratorImpl;
  std::string text = "   !:,213213Hello,]]][[[[    World  !   ";
  AlphaFilter filter;
  std::string result;
  for (IteratorImpl it(text.begin(), text.end(), filter, LowerAndSpace), end; it != end; ++it) {
    result += static_cast<char>(*it);
  }
  BOOST_CHECK(result == " hello world ");
}

BOOST_AUTO_TEST_CASE(Utility_SymbolAlphaIterator_RussianGeneral) {
  typedef encode::Utf8Iterator<std::string::const_iterator> Utf8IteratorImpl;
  typedef FilterIterator<symbols::SymbolCode, Utf8IteratorImpl, AlphaFilter, SymTransform> IteratorImpl;
  std::string text = "    12321321  ,,, \\///!!! Здравствуй, [[[ Мир  ]]]  !!!!";
  AlphaFilter filter;
  std::string result;
  Utf8IteratorImpl utf8_begin(text.begin(), text.end()), utf8_end;
  for (IteratorImpl it(utf8_begin, utf8_end, filter, LowerAndSpace), end; it != end; ++it) {
    encode::GetUtf8Sequence(*it, std::back_inserter(result));
  }
  BOOST_CHECK(result == " здравствуй мир ");
}

BOOST_AUTO_TEST_CASE(Utility_WordIterator_EnglishRussianWords) {
  typedef encode::Utf8Iterator<std::string::const_iterator> Utf8IteratorImpl;
  typedef WordIterator<symbols::SymbolCode, Utf8IteratorImpl> WordIteratorImpl;
  std::string text = "    12321321  ,,, \\///!!! Здравствуй, hello [[[ Мир  ]]]  !!!! World";
  Utf8IteratorImpl utf8_begin(text.begin(), text.end()), utf8_end;
  size_t counter = 1;
  for (WordIteratorImpl it(utf8_begin, utf8_end), end; it != end; ++it, ++counter) {
    std::string result;
    encode::GetUtf8Sequence((*it).begin(), (*it).end(), std::back_inserter(result));
    switch (counter) {
      case 1:
        BOOST_CHECK(result == "Здравствуй");
        break;
      case 2:
        BOOST_CHECK(result == "hello");
        break;
      case 3:
        BOOST_CHECK(result == "Мир");
        break;
      case 4:
        BOOST_CHECK(result == "World");
        break;
      default:
        BOOST_ERROR("Incorrect number of words extracted");
        break;
    }
  }
}

}} // namespace strutext, utility.
