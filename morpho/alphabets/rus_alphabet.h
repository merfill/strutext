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
 * \brief  Russian alphabet defintion.
 * \author Vladimir Lapshin.
 */

#pragma once

#include "alphabet.h"

namespace strutext { namespace morpho {

/// Russian alphabet implementation.
struct RussianAlphabet : public Alphabet<RussianAlphabet> {
  /// Encoding implementation.
  Code EncodeImpl(const Symbol& symbol) const {
    // Interval [а;я].
    if (symbol >= 0x0430 and symbol <= 0x044f) {
      return symbol - 0x0430 + 1;
    }
    // 'ё' symbol.
    if (symbol == 0x0451) {
      return 33;
    }

    // 0 is bad code.
    return 0;
  }

  /// Encoding implementation.
  Symbol DecodeImpl(const Code& code) const {
    // Interval [а;я].
    if (code >= 1 and code <= 32) {
      return 0x0430 + code - 1;
    }
    // 'ё' symbol.
    if (code == 33) {
      return 0x0451;
    }

    // 0 is bad symbol.
    return 0;
  }

  /// Get number of symbols in alphabet.
  size_t SizeImpl() const {
    return 33;
  }
};

}} // namespace strutext, morpho.
