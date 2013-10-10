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
 * \brief  English alphabet defintion.
 * \author Vladimir Lapshin.
 */

#pragma once

#include "alphabet.h"

namespace strutext { namespace morpho {

/// Russian alphabet implementation.
struct EnglishAlphabet : public Alphabet<EnglishAlphabet> {
  /// Encoding implementation.
  Code EncodeImpl(const Symbol& symbol) const {
    if (symbol >= 0x61 and symbol <= 0x7a) {
      return symbol;
    }
    // 0 is bad code.
    return 0;
  }

  /// Encoding implementation.
  Symbol DecodeImpl(const Code& code) const {
    if (code >= 0x61 and code <= 0x7a) {
      return code;
    }
    // 0 is bad symbol.
    return 0;
  }

  /// Get number of symbols in alphabet.
  size_t SizeImpl() const {
    return 26;
  }
};

}} // namespace strutext, morpho.
