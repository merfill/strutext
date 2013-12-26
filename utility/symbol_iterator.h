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
 * \brief  Symbol stream iterator droping nonaplhas and lowering symbols.
 * \author Vladimir Lapshin.
 */

#pragma once

#include "filter_iterator.h"
#include "symbols.h"

namespace strutext { namespace utility {

/// Nonalphas droping implementation.
struct AlphaFilter {
  AlphaFilter() : is_space_read_(false) {}

  bool operator()(symbols::SymbolCode symbol) {
    if (symbols::IsSeparator(symbol)) {
      if (not is_space_read_) {
        is_space_read_ = true;
        return true;
      } else {
        return false;
      }
    } else if (symbols::IsLetter(symbol)) {
      is_space_read_ = false;
      return true;
    }
    return false;
  }

  bool is_space_read_;
};

/// Lowering symbols.
inline symbols::SymbolCode LowerAndSpace(symbols::SymbolCode symbol) {
  if (symbols::IsSeparator(symbol)) {
    return static_cast<symbols::SymbolCode>(0x20);
  }
  return symbols::ToLower(symbol);
}

typedef symbols::SymbolCode (*SymTransform)(symbols::SymbolCode);

}} // namespace strutext, utility.
