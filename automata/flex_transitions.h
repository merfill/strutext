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
 * \brief  Flexible moving table implementation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <map>

#include "fsm_defs.h"

namespace strutext { namespace automata {

/// Serializer forward declaration.
template <class TransImpl> class TransSerializer;

/**
 * \brief "Flexible" move table.
 *
 * "Flexible" move table is implemented by using std::map.
 */
template <typename Char>
struct FlexTransitions {
  /// Symbol type.
  typedef Char CharType;

  // Move table type.
  typedef std::map<CharType, StateId> TransTable;

  // Make serializer to be friend.
  friend class TransSerializer<FlexTransitions<Char> >;

  /**
   * \brief Move implementation.
   *
   * \param  symbol Symbol to make moving.
   * \return        Result state index or kInvalidState.
   */
  StateId Go(const CharType& symbol) const {
    typename TransTable::const_iterator it = trans_table_.find(symbol);
    if (it != trans_table_.end()) {
      return (*it).second;
    }
    return kInvalidState;
  }

  /**
   * \brief Adding move.
   *
   *  \param  to     State index to move to.
   *  \param  symbol Symbol to make moving.
   */
  void AddTransition(const StateId& to, const CharType& symbol) {
    trans_table_[symbol] = to;
  }

  /// Return reference to the move table.
  TransTable GetMoveTable() const {
    return trans_table_;
  }

  TransTable trans_table_; ///< Move table.
};

}} // namespace strutext, automata.
