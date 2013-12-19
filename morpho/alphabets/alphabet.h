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
 * \brief  Alphabet interface defintion.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace strutext { namespace morpho {

/**
 * \brief Alphabet interface definition.
 *
 * Implementation of alphabet of the language implemented.
 *
 * Alphabet class implements static virtuality pattern based on CRTP
 * [http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern]. Users must
 * implement class with class methods marked by "Impl" suffix and inherit the
 * implementation from Encoder class.
 * This only work when alphabet size is <= 255.
 */
template <class EncoderImpl>
struct Alphabet {
  /// Code type definition.
  typedef char Code;

  /// Symbol type definition.
  typedef uint32_t Symbol;

  /// Encoding implementation.
  Code Encode(const Symbol& symbol) const {
    return static_cast<const EncoderImpl*>(this)->EncodeImpl(symbol);
  }

  /// Encoding implementation.
  Symbol Decode(const Code& code) const {
    return static_cast<const EncoderImpl*>(this)->DecodeImpl(code);
  }

  /// Get number of symbols in alphabet.
  size_t Size() const {
    return static_cast<const EncoderImpl*>(this)->SizeImpl();
  }
};

}} // namespace strutext, morpho.
