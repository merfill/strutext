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
 * \brief  Flat move table implementation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include <map>

#include "fsm_defs.h"

namespace strutext { namespace automata {

/// Serializer forward declaration.
template <class TransImpl> class TransSerializer;

/**
 * \brief "Flat" move table implementation.
 *
 * Flat table is just the matrix of Size symbols in each row and row count
 * equals to number of states. To do moving from state_id by symbol it's needed to
 * look at table element [state_id, symbol]. If there is not move [state_id, symbol]
 * contains invalid state id.
 *
 * Template parameters:
 *   Char: the symbol type.
 *   Size: the size of move table.
 */
template <typename Char, size_t Size >
struct FlatTransitions {
  /// Symbol type.
  typedef Char CharType;

  /// Move table type.
  typedef std::map<CharType, StateId> TransTable;

  // Make serializer to be friend.
  friend class TransSerializer<FlatTransitions<Char, Size> >;

  /// Default initilization.
  FlatTransitions() {
    // Fill all table elements by invalid state ids.
    std::fill(trans_table_, trans_table_ + Size, kInvalidState);
  }

  /**
   * \brief Move implementation.
   *
   * \param  symbol Symbol to make move.
   * \return        The index of the move state or kInvalidState if the move is absent.
   */
  StateId Go(CharType symbol) const {
    assert(symbol < Size);
    return trans_table_[symbol];
  }

  /**
   * \brief Adding new move.
   *
   *  \param  to     The index of a state to move to.
   *  \param  symbol The move symbol.
   */
  void AddTransition(const automata::StateId& to, const CharType& symbol) {
    assert(symbol < Size);
    trans_table_[symbol] = to;
  }

  /// Return reference to move table.
  TransTable GetMoveTable() const {
    TransTable result;
    for (size_t i = 0; i < Size; ++i) {
      if (trans_table_[i] > 0) {
        result.insert(std::make_pair(static_cast<CharType>(i), trans_table_[i]));
      }
    }
    return result;
  }

  StateId trans_table_[Size]; ///< Move table.
};

}} // namespace strutext, automata.
