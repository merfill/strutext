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
 * \brief  Regular expression parser implementation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include "symbol_set.h"

namespace strutext { namespace re {

/**
 * \brief Regular expression parser implementation.
 *
 * The parser builds Nondeterministic Finite Automaton (NFA) basing on passed RE text.
 * Template parameters:
 *   T1 -- Symbol iterator type. The iterator must return UNICODE symbols.
 *   T2 -- Symbol set type. Each RE parser operates finite set of symbols, as encountered
 *         by '.' operator.
 */
template <typename T1, class T2>
class ReParser {
public:
  /// Symbol iterator type.
  typedef T1 SymbolIterator;

  /// Symbol set implementation.
  typedef SymbolSet<T2> SymbolSetImpl;

  /**
   * \brief Parse passed RE text to produce NFA as the result.
   *
   * \param  begin Begin of the symbol stream.
   * \param  end   End of the symbol stream.
   * \return       Result NFA.
   */

};

}} // namespace strutext, re.
