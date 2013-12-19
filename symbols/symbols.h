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
 * \brief  Symbol classes and operations.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

namespace strutext { namespace symbols {

/// The symbol code type.
typedef uint32_t SymbolCode;

/**
 * \brief The symbol class type definition.
 *
 * The definition of information concerning UNICODE symbol. An each
 * UNICODE symbol must have class. Possible classes are:
 *   Control:    any symbol, which may be used control text out.
 *   Letter:     letters in any language.
 *   Number:     digits in any language.
 *   Separator:  symbol, which may be used as separator in texts (space as example).
 *   Punctuator: the punctuation symbol -- "!,;" and etc.
 *   Mark:       for example, enclosing square, which is used to sign theorem proving.
 *   Symbol:     for example, '$' sign.
 * The classes may also have sybclasses. See type definition for more information.
 * The SymbolInfo type provides information concerning symbol class and some
 * extra information, which depends on the symbol class. For instance, for
 * Letter symbol class the type contains code of lower and upper letter variant.
 */
enum SymbolClass {
  UPPERCASE_LETTER      = 0x00000001,
  LOWERCASE_LETTER      = 0x00000002,
  TITLECASE_LETTER      = 0x00000004,
  CASED_LETTER          = UPPERCASE_LETTER | LOWERCASE_LETTER | TITLECASE_LETTER,
  MODIFIER_LETTER       = 0x00000008,
  OTHER_LETTER          = 0x00000010,
  LETTER                = CASED_LETTER | MODIFIER_LETTER | OTHER_LETTER,
  NONSPACING_MARK       = 0x00000020,
  SPACING_MARK          = 0x00000040,
  ENCLOSING_MARK        = 0x00000080,
  MARK                  = NONSPACING_MARK | SPACING_MARK | ENCLOSING_MARK,
  DECIMAL_NUMBER        = 0x00000100,
  LETTER_NUMBER         = 0x00000200,
  OTHER_NUMBER          = 0x00000400,
  NUMBER                = DECIMAL_NUMBER | LETTER_NUMBER | OTHER_NUMBER,
  CONNECTOR_PUNCTUATION = 0x00000800,
  DASH_PUNCTUATION      = 0x00001000,
  OPEN_PUNCTUATION      = 0x00002000,
  CLOSE_PUNCTUATION     = 0x00004000,
  INITIAL_PUNCTUATION   = 0x00008000,
  FINAL_PUNCTUATION     = 0x00010000,
  OTHER_PUNCTUATION     = 0x00020000,
  PUNCTUATION           = CONNECTOR_PUNCTUATION | DASH_PUNCTUATION | OPEN_PUNCTUATION | CLOSE_PUNCTUATION
                          | INITIAL_PUNCTUATION | FINAL_PUNCTUATION | OTHER_PUNCTUATION,
  MATH_SYMBOL           = 0x00040000,
  CURRENCY_SYMBOL       = 0x00080000,
  MODIFIER_SYMBOL       = 0x00100000,
  OTHER_SYMBOL          = 0x00200000,
  SYMBOL                = MATH_SYMBOL | CURRENCY_SYMBOL | MODIFIER_SYMBOL | OTHER_SYMBOL,
  SPACE_SEPARATOR       = 0x00400000,
  LINE_SEPARATOR        = 0x00800000,
  PARAGRAPH_SEPARATOR   = 0x01000000,
  SEPARATOR             = SPACE_SEPARATOR | LINE_SEPARATOR | PARAGRAPH_SEPARATOR,
  CONTROL               = 0x02000000,
  FORMAT                = 0x04000000,
  SURROGATE             = 0x08000000,
  PRIVATE_USE           = 0x10000000,
  UNASSIGNED            = 0x20000000,
  OTHER                 = CONTROL | FORMAT | SURROGATE | PRIVATE_USE | UNASSIGNED
};

namespace details {

// The maximum symbol code.
static const uint32_t SYMBOL_TABLE_SIZE = 0x200000;

// The symbol tables declarations.
extern uint32_t    SYM_CLASS_TABLE[];
extern SymbolCode  SYM_UPPER_TABLE[];
extern SymbolCode  SYM_LOWER_TABLE[];

}  // namespace details.

inline const uint32_t& GetSymbolClass(const SymbolCode& code) {
  return details::SYM_CLASS_TABLE[code];
}

inline SymbolCode ToLower(const SymbolCode& code) {
  return details::SYM_LOWER_TABLE[code];
}

inline SymbolCode ToUpper(const SymbolCode& code) {
  return details::SYM_UPPER_TABLE[code];
}

template<SymbolClass class_name>
inline bool Is(const SymbolCode& code)  {
  return static_cast<uint32_t>(class_name) & GetSymbolClass(code);
}

inline bool IsCasedLetter(const SymbolCode& code) {
  return Is<CASED_LETTER>(code);
}

inline bool IsLetter(const SymbolCode& code) {
  return Is<LETTER>(code);
}

inline bool IsMark(const SymbolCode& code) {
  return Is<MARK>(code);
}

inline bool IsNumber(const SymbolCode& code) {
  return Is<NUMBER>(code);
}

inline bool IsPunctuation(const SymbolCode& code) {
  return Is<PUNCTUATION>(code);
}

inline bool IsSymbol(const SymbolCode& code) {
  return Is<SYMBOL>(code);
}

inline bool IsSeparator(const SymbolCode& code) {
  return Is<SEPARATOR>(code);
}

inline bool IsOther(const SymbolCode& code) {
  return Is<OTHER>(code);
}

// Some fundamental UNICODE constants.
const SymbolCode MAX_UTF32        = 0x7fffffffu;
const SymbolCode MAX_LEGAL_UTF32  = 0x0010ffffu;
const SymbolCode MAX_UTF16        = 0x0010ffffu;

}} // namespace strutext, symbols.
