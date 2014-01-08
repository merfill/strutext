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
 * \brief  ANSI symbol set unit test.
 * \author Vladimir Lapshin.
 */

#include <boost/test/unit_test.hpp>

#include "ansi_symbol_set.h"

namespace strutext { namespace re {

typedef SymbolSet<AnsiSymbolSet> SymbolSetImpl;

BOOST_AUTO_TEST_CASE(Re_AnsiSymbolSet_Encode) {
  SymbolSetImpl ss;

  SymbolSetImpl::SymbolImpl symbol = ss.Encode('*');
  BOOST_CHECK(symbol.code_ == '*');
  BOOST_CHECK(symbol.class_ == STAR_SC);

  symbol = ss.Encode('+');
  BOOST_CHECK(symbol.code_ == '+');
  BOOST_CHECK(symbol.class_ == PLUS_SC);

  symbol = ss.Encode('?');
  BOOST_CHECK(symbol.code_ == '?');
  BOOST_CHECK(symbol.class_ == QUESTION_SC);

  symbol = ss.Encode('.');
  BOOST_CHECK(symbol.code_ == '.');
  BOOST_CHECK(symbol.class_ == DOT_SC);

  symbol = ss.Encode('[');
  BOOST_CHECK(symbol.code_ == '[');
  BOOST_CHECK(symbol.class_ == OPEN_BRAKET_SC);

  symbol = ss.Encode(']');
  BOOST_CHECK(symbol.code_ == ']');
  BOOST_CHECK(symbol.class_ == CLOSE_BRAKET_SC);

  symbol = ss.Encode('(');
  BOOST_CHECK(symbol.code_ == '(');
  BOOST_CHECK(symbol.class_ == OPEN_PAREN_SC);

  symbol = ss.Encode(')');
  BOOST_CHECK(symbol.code_ == ')');
  BOOST_CHECK(symbol.class_ == CLOSE_PAREN_SC);

  symbol = ss.Encode('{');
  BOOST_CHECK(symbol.code_ == '{');
  BOOST_CHECK(symbol.class_ == OPEN_BRACE_SC);

  symbol = ss.Encode('}');
  BOOST_CHECK(symbol.code_ == '}');
  BOOST_CHECK(symbol.class_ == CLOSE_BRACE_SC);

  symbol = ss.Encode('-');
  BOOST_CHECK(symbol.code_ == '-');
  BOOST_CHECK(symbol.class_ == DASH_SC);

  symbol = ss.Encode('\\');
  BOOST_CHECK(symbol.code_ == '\\');
  BOOST_CHECK(symbol.class_ == BACKSLASH_SC);

  symbol = ss.Encode('|');
  BOOST_CHECK(symbol.code_ == '|');
  BOOST_CHECK(symbol.class_ == ALTER_SC);

  std::string number = "0123456789";
  for (std::string::const_iterator it = number.begin(); it != number.end(); ++it) {
    symbol = ss.Encode(*it);
    BOOST_CHECK(symbol.code_ == *it);
    BOOST_CHECK(symbol.class_ == SYMBOL_SC);
  }

  std::string alphas = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBN";
  for (std::string::const_iterator it = alphas.begin(); it != alphas.end(); ++it) {
    symbol = ss.Encode(*it);
    BOOST_CHECK(symbol.code_ == *it);
    BOOST_CHECK(symbol.class_ == SYMBOL_SC);
  }

  std::string controls = "\x00\t\n\r\v";
  for (std::string::const_iterator it = controls.begin(); it != controls.end(); ++it) {
    symbol = ss.Encode(*it);
    BOOST_CHECK(symbol.code_ == *it);
    BOOST_CHECK(symbol.class_ == SYMBOL_SC);
  }

  std::string separators = " ,;:!";
  for (std::string::const_iterator it = separators.begin(); it != separators.end(); ++it) {
    symbol = ss.Encode(*it);
    BOOST_CHECK(symbol.code_ == *it);
    BOOST_CHECK(symbol.class_ == SYMBOL_SC);
  }
}

BOOST_AUTO_TEST_CASE(Re_AnsiSymbolSet_Decode) {
  SymbolSetImpl ss;
  for (symbols::SymbolCode symbol = 0; symbol <= 0x7f; ++symbol) {
    switch (symbol) {
      case '*':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl('*', STAR_SC)) == symbol);
        break;
      case '+':
        BOOST_CHECK( ss.Decode(SymbolSetImpl::SymbolImpl('+', PLUS_SC)) ==symbol);
        break;
      case '?':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl('?', QUESTION_SC)) == symbol);
        break;
      case '.':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl('.', DOT_SC)) == symbol);
        break;
      case '[':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl('[', OPEN_BRAKET_SC)) == symbol);
        break;
      case ']':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl(']', CLOSE_BRAKET_SC)) == symbol);
        break;
      case '(':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl('(', OPEN_PAREN_SC)) == symbol);
        break;
      case ')':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl(')', CLOSE_PAREN_SC)) == symbol);
        break;
      case '{':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl('{', OPEN_BRACE_SC)) == symbol);
        break;
      case '}':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl('}', CLOSE_BRACE_SC)) == symbol);
        break;
      case '-':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl('-', DASH_SC)) == symbol);
        break;
      case '\\':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl('\\', BACKSLASH_SC)) == symbol);
        break;
      case '|':
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl('|', ALTER_SC)) == symbol);
        break;
      default:
        BOOST_CHECK(ss.Decode(SymbolSetImpl::SymbolImpl(static_cast<SymbolSetImpl::SymbolCodeImpl>(symbol), SYMBOL_SC))
                      == symbol);
    }
  }
}

BOOST_AUTO_TEST_CASE(Re_AnsiSymbolSet_SymbolSet) {
  SymbolSetImpl ss;
  const SymbolSetImpl::SymbolCodeSet& symbol_set = ss.GetSymbolSet();
  BOOST_REQUIRE(symbol_set.size() == 128);
  for (uint8_t symbol = 0; symbol <= 0x7f; ++symbol) {
    BOOST_CHECK(symbol_set.find(symbol) != symbol_set.end());
  }
}

BOOST_AUTO_TEST_CASE(Re_AnsiSymbolSet_EscapeSequences) {
  SymbolSetImpl ss;

  std::string text = "\\t";
  size_t num_of_symbols = 0;
  SymbolSetImpl::SymbolImpl symbol = ss.ExtractEscapeSequence(text.begin(), text.end(), num_of_symbols);
  BOOST_CHECK(symbol.class_ == SYMBOL_SC);
  BOOST_CHECK(symbol.code_ == '\t');
  BOOST_CHECK(num_of_symbols == 1);

  text = "\\n";
  num_of_symbols = 0;
  symbol = ss.ExtractEscapeSequence(text.begin(), text.end(), num_of_symbols);
  BOOST_CHECK(symbol.class_ == SYMBOL_SC);
  BOOST_CHECK(symbol.code_ == '\n');
  BOOST_CHECK(num_of_symbols == 1);

  text = "\\v";
  num_of_symbols = 0;
  symbol = ss.ExtractEscapeSequence(text.begin(), text.end(), num_of_symbols);
  BOOST_CHECK(symbol.class_ == SYMBOL_SC);
  BOOST_CHECK(symbol.code_ == '\v');
  BOOST_CHECK(num_of_symbols == 1);

  text = "\\r";
  num_of_symbols = 0;
  symbol = ss.ExtractEscapeSequence(text.begin(), text.end(), num_of_symbols);
  BOOST_CHECK(symbol.class_ == SYMBOL_SC);
  BOOST_CHECK(symbol.code_ == '\r');
  BOOST_CHECK(num_of_symbols == 1);

  text = "\\x20";
  num_of_symbols = 0;
  symbol = ss.ExtractEscapeSequence(text.begin(), text.end(), num_of_symbols);
  BOOST_CHECK(symbol.class_ == SYMBOL_SC);
  BOOST_CHECK(symbol.code_ == 0x20);
  BOOST_CHECK(num_of_symbols == 3);

  try {
    text = "\\x80";
    num_of_symbols = 0;
    symbol = ss.ExtractEscapeSequence(text.begin(), text.end(), num_of_symbols);
    BOOST_ERROR("Must not be here");
  } catch (const std::invalid_argument& err) {
    BOOST_CHECK(err.what() == std::string("incorrect escape sequence"));
  }
}

}} // namespace strutext, re.
