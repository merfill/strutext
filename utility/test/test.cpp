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

#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

#include "ngram_iterator.h"

namespace strutext { namespace utility {

BOOST_AUTO_TEST_CASE(NgrammIterator_Symbols_Ansii) {
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

}} // namespace strutext, utility.
