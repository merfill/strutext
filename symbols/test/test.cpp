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
 * \brief  The symbols library unit test.
 * \author Vladimir Lapshin.
 * \date   08.10.2013
 */

#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include "symbols.h"

namespace sym = strutext::symbols;

BOOST_AUTO_TEST_CASE(Symbols_SymbolClass_EnglishLetters) {
  // qwertyuiopasdfghjklzxcvbnm
  const sym::SymbolCode letters[] = {
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm'
  };
  BOOST_FOREACH(sym::SymbolCode c, letters) {
    BOOST_CHECK(sym::IsLetter(c));
    BOOST_CHECK(sym::Is<sym::LETTER>(c));
  }
}

BOOST_AUTO_TEST_CASE(Symbols_SymbolClass_RussianLetters) {
  // абвгдежзийклмнопрстуфхцчшщъыьэюяё
  const sym::SymbolCode letters[] = {
    0x430, 0x431, 0x432, 0x433, 0x434, 0x435, 0x436, 0x437, 0x438, 0x439, 0x43a, 0x43b,
    0x43c, 0x43d, 0x43e, 0x43f, 0x440, 0x441, 0x442, 0x443, 0x444, 0x445, 0x446, 0x447,
    0x448, 0x449, 0x44a, 0x44b, 0x44c, 0x44d, 0x44e, 0x44f, 0x451
  };
  BOOST_FOREACH(sym::SymbolCode c, letters) {
    BOOST_CHECK(sym::IsLetter(c));
    BOOST_CHECK(sym::Is<sym::LETTER>(c));
  }
}

BOOST_AUTO_TEST_CASE(Symbols_SymbolClass_Digits) {
  // 01234567890
  const sym::SymbolCode digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
  BOOST_FOREACH(sym::SymbolCode c, digits) {
    BOOST_CHECK(sym::IsNumber(c));
    BOOST_CHECK(sym::Is<sym::NUMBER>(c));
  }
}

BOOST_AUTO_TEST_CASE(Symbols_SymbolClass_Punctuators) {
  // +-@$%^&?=!
  const sym::SymbolCode punctuators[] = {
    '.', ':', ';', ',', '!', '?', '#', '%', '-', '\'', '"', '@', '(', ')', '[', ']', '*', '/', '\\', '&'
  };
  BOOST_FOREACH(sym::SymbolCode c, punctuators) {
    BOOST_CHECK(sym::IsPunctuation(c));
    BOOST_CHECK(sym::Is<sym::PUNCTUATION>(c));
  }
}

BOOST_AUTO_TEST_CASE(Symbols_SymbolCases_EnglishToUpper) {
  // qwertyuiopasdfghjklzxcvbnm
  const sym::SymbolCode lowers[] = {
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm'
  };
  // QWERTYUIOPASDFGHJKLZXCVBNM
  const sym::SymbolCode uppers[] = {
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H',
    'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M'
  };
  size_t current = 0;
  BOOST_FOREACH(sym::SymbolCode c, lowers) {
    BOOST_CHECK(sym::IsCasedLetter(c));
    BOOST_CHECK(sym::ToUpper(c) == uppers[current]);
    ++current;
  }
}

BOOST_AUTO_TEST_CASE(Symbols_SymbolCases_EnglishToLower) {
  // qwertyuiopasdfghjklzxcvbnm
  const sym::SymbolCode lowers[] = {
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm'
  };
  // QWERTYUIOPASDFGHJKLZXCVBNM
  const sym::SymbolCode uppers[] = {
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H',
    'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M'
  };
  size_t current = 0;
  BOOST_FOREACH(sym::SymbolCode c, lowers) {
    BOOST_CHECK(sym::IsCasedLetter(c));
    BOOST_CHECK(sym::ToUpper(c) == uppers[current]);
    ++current;
  }
}

BOOST_AUTO_TEST_CASE(Symbols_SymbolCases_RussianToUpper) {
  // абвгдежзийклмнопрстуфхцчшщъыьэюяё
  const sym::SymbolCode lowers[] = {
    0x430, 0x431, 0x432, 0x433, 0x434, 0x435, 0x436, 0x437, 0x438, 0x439, 0x43a, 0x43b,
    0x43c, 0x43d, 0x43e, 0x43f, 0x440, 0x441, 0x442, 0x443, 0x444, 0x445, 0x446, 0x447,
    0x448, 0x449, 0x44a, 0x44b, 0x44c, 0x44d, 0x44e, 0x44f, 0x451
  };
  // АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯЁ
  const sym::SymbolCode uppers[] = {
    0x410, 0x411, 0x412, 0x413, 0x414, 0x415, 0x416, 0x417, 0x418, 0x419, 0x41a, 0x41b,
    0x41c, 0x41d, 0x41e, 0x41f, 0x420, 0x421, 0x422, 0x423, 0x424, 0x425, 0x426, 0x427,
    0x428, 0x429, 0x42a, 0x42b, 0x42c, 0x42d, 0x42e, 0x42f, 0x401
  };
  size_t current = 0;
  BOOST_FOREACH(sym::SymbolCode c, lowers) {
    BOOST_CHECK(sym::IsCasedLetter(c));
    BOOST_CHECK(sym::ToUpper(c) == uppers[current]);
    ++current;
  }
}

BOOST_AUTO_TEST_CASE(Symbols_SymbolCases_RussianToLower) {
  // абвгдежзийклмнопрстуфхцчшщъыьэюяё
  const sym::SymbolCode lowers[] = {
    0x430, 0x431, 0x432, 0x433, 0x434, 0x435, 0x436, 0x437, 0x438, 0x439, 0x43a, 0x43b,
    0x43c, 0x43d, 0x43e, 0x43f, 0x440, 0x441, 0x442, 0x443, 0x444, 0x445, 0x446, 0x447,
    0x448, 0x449, 0x44a, 0x44b, 0x44c, 0x44d, 0x44e, 0x44f, 0x451
  };
  // АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯЁ
  const sym::SymbolCode uppers[] = {
    0x410, 0x411, 0x412, 0x413, 0x414, 0x415, 0x416, 0x417, 0x418, 0x419, 0x41a, 0x41b,
    0x41c, 0x41d, 0x41e, 0x41f, 0x420, 0x421, 0x422, 0x423, 0x424, 0x425, 0x426, 0x427,
    0x428, 0x429, 0x42a, 0x42b, 0x42c, 0x42d, 0x42e, 0x42f, 0x401
  };
  size_t current = 0;
  BOOST_FOREACH(sym::SymbolCode c, uppers) {
    BOOST_CHECK(sym::IsCasedLetter(c));
    BOOST_CHECK(sym::ToLower(c) == lowers[current]);
    ++current;
  }
}
