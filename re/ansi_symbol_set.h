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
 * \brief  Symbol set for ANSI encoding.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <cstddef>
#include <stdexcept>

#include "symbol_set.h"

namespace strutext { namespace re {

/// Base symbol set class definition.
struct AnsiSymbolSet {
  /// Symbol code type.
  typedef uint8_t SymbolCode;

  AnsiSymbolSet() {
    for (uint8_t symbol = 0; symbol <= 127; ++symbol) {
      symbol_set_.insert(symbol);
    }
  }

  /**
   * \brief Decoding input UNICODE symbol to internal representation.
   *
   * \param  symbol UNICODE symbol.
   * \return        Symbol code and class information.
   */
  Symbol<SymbolCode> EncodeImpl(symbols::SymbolCode symbol) const {
    switch (symbol) {
      case '*':
        return Symbol<SymbolCode>('*', STAR_SC);
      case '+':
        return Symbol<SymbolCode>('+', PLUS_SC);
      case '?':
        return Symbol<SymbolCode>('?', QUESTION_SC);
      case '.':
        return Symbol<SymbolCode>('.', DOT_SC);
      case '[':
        return Symbol<SymbolCode>('[', OPEN_BRAKET_SC);
      case ']':
        return Symbol<SymbolCode>(']', CLOSE_BRAKET_SC);
      case '(':
        return Symbol<SymbolCode>('(', OPEN_PAREN_SC);
      case ')':
        return Symbol<SymbolCode>(')', CLOSE_PAREN_SC);
      case '{':
        return Symbol<SymbolCode>('{', OPEN_BRACE_SC);
      case '}':
        return Symbol<SymbolCode>('}', CLOSE_BRACE_SC);
      case '-':
        return Symbol<SymbolCode>('-', DASH_SC);
      case '\\':
        return Symbol<SymbolCode>('\\', BACKSLASH_SC);
      case '|':
        return Symbol<SymbolCode>('|', ALTER_SC);
      default:
        if (symbol <= 127) {
          return Symbol<SymbolCode>(static_cast<SymbolCode>(symbol), SYMBOL_SC);
        }
    }
    return Symbol<SymbolCode>(0, UNKNOWN_SC);
  }

  /**
   * \brief Decode symbol internal representation to UNICODE.
   *
   * \param  symbol Internal symbol rpresentation.
   * \return        UNICODE symbol representation.
   */
  symbols::SymbolCode DecodeImpl(const Symbol<SymbolCode>& symbol) const {
    return static_cast<symbols::SymbolCode>(symbol.code_);
  }

  /// Get symbol code set.
  const std::set<SymbolCode>& GetSymbolSetImpl() const {
    return symbol_set_;
  }

  /**
   * \brief Escape sequence extraction.
   *
   * \param      begin          Begin iterator of symbol stream.
   * \param      end            End iterator of symbol stream.
   * \param[out] num_of_symbols Number of symbols read.
   * \return                    Read symbol.
   */
  template <class SymbolIterator>
  Symbol<SymbolCode> ExtractEscapeSequenceImpl(SymbolIterator begin, SymbolIterator end, size_t& num_of_symbols) const {
    num_of_symbols = 0;
    SymbolCode result = 0;
    SymbolIterator it = begin;
    if (*it != '\\') {
      throw std::invalid_argument("first symbol is not escape");
    }
    if (++it == end) {
      throw std::invalid_argument("incomplete escape sequence");
    }

    switch (*it) {
      case 't':
        num_of_symbols = 1;
        return Symbol<SymbolCode>(static_cast<SymbolCode>('\t'), SYMBOL_SC);
      case 'n':
        num_of_symbols = 1;
        return Symbol<SymbolCode>(static_cast<SymbolCode>('\n'), SYMBOL_SC);
      case 'r':
        num_of_symbols = 1;
        return Symbol<SymbolCode>(static_cast<SymbolCode>('\r'), SYMBOL_SC);
      case 'v':
        num_of_symbols = 1;
        return Symbol<SymbolCode>(static_cast<SymbolCode>('\v'), SYMBOL_SC);
      case 'x':
        num_of_symbols = 1;
        for (size_t i = 0; i < 2; ++i) {
          ++num_of_symbols;
          if (++it == end) {
            throw std::invalid_argument("incomplete escape sequence");
          } else if (*it >= '0' and *it <= '9') {
            result = result * 16 + static_cast<SymbolCode>(*it - '0');
          } else {
            throw std::invalid_argument("incorrect escape sequence");
          }
        }
        if (result > 127) {
          throw std::invalid_argument("incorrect escape sequence");
        }
        break;
      default:
        num_of_symbols = 1;
        return Symbol<SymbolCode>(static_cast<SymbolCode>(*it), SYMBOL_SC);
    }

    return Symbol<SymbolCode>(result, SYMBOL_SC);
  }

private:
  std::set<SymbolCode> symbol_set_; ///< ANSI symbol set;
};

}} // namespace strutext, re.
