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
 * \brief  Symbol classes for regular expression parser.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <cstddef>
#include <set>

#include "symbols.h"

namespace strutext { namespace re {

/// Symbol class definitions.
enum SymbolClass {
  UNKNOWN_SC = 0
  , SYMBOL_SC           ///< Ordinar symbol.
  , STAR_SC             ///< '*' operator.
  , PLUS_SC             ///< '+' operator.
  , QUESTION_SC         ///< '?' operator.
  , DOT_SC              ///< '.' operator.
  , OPEN_BRAKET_SC      ///< '['
  , CLOSE_BRAKET_SC     ///< ']'
  , OPEN_PAREN_SC       ///< '('
  , CLOSE_PAREN_SC      ///< ')'
  , OPEN_BRACE_SC       ///< '{'
  , CLOSE_BRACE_SC      ///< '}'
  , DASH_SC             ///< '-'
  , BACKSLASH_SC        ///< '\'
  , ALTER_SC            ///< '|'
};

/// Symbol implementation.
template <typename T>
struct Symbol {
  /// Type of the symbol code.
  typedef T SymbolCode;

  Symbol()
    : code_()
    , class_(UNKNOWN_SC) {}

  Symbol(SymbolCode code, SymbolClass cls)
    : code_(code)
    , class_(cls) {}

  SymbolCode  code_;  ///< Code of the symbol.
  SymbolClass class_; ///< Class of the symbol.
};

/// Base symbol set class definition.
template <class SymbolSetImpl>
struct SymbolSet : public SymbolSetImpl {
  /// Symbol code type.
  typedef typename SymbolSetImpl::SymbolCode SymbolCodeImpl;

  /// Symbol type.
  typedef Symbol<SymbolCodeImpl> SymbolImpl;

  /// Type of symbol code set.
  typedef std::set<SymbolCodeImpl> SymbolCodeSet;

  /**
   * \brief Decoding input UNICODE symbol to internal representation.
   *
   * \param  symbol UNICODE symbol.
   * \return        Symbol code and class information.
   */
  SymbolImpl Encode(symbols::SymbolCode symbol) const {
    return static_cast<const SymbolSetImpl*>(this)->EncodeImpl(symbol);
  }

  /**
   * \brief Decode symbol internal representation to UNICODE.
   *
   * \param  symbol Internal symbol rpresentation.
   * \return        UNICODE symbol representation.
   */
  symbols::SymbolCode Decode(const SymbolImpl& symbol) const {
    return static_cast<const SymbolSetImpl*>(this)->DecodeImpl(symbol);
  }

  /// Get symbol code set.
  const SymbolCodeSet& GetSymbolSet() const {
    return static_cast<const SymbolSetImpl*>(this)->GetSymbolSetImpl();
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
  SymbolImpl ExtractEscapeSequence(SymbolIterator begin, SymbolIterator end, size_t& num_of_symbols) const {
    return static_cast<const SymbolSetImpl*>(this)->ExtractEscapeSequenceImpl(begin, end, num_of_symbols);
  }
};

}} // namespace strutext, re.
