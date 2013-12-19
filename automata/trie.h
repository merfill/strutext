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
 * \brief  Trie imlementation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include "attr_fsm.h"

namespace strutext { namespace automata {

/// Trie implementation basing on attribute FSM.
template <class Trans, typename Attribute>
struct Trie : public AttributeFsm<Trans, Attribute> {
  /// Chain identifier type.
  typedef Attribute ChainId;

  /// Attribute FSM type.
  typedef AttributeFsm<Trans, Attribute> AttributeFsmImpl;

  /// Default initialization.
  explicit Trie(size_t rsize = AttributeFsmImpl::kReservedStateTableSize) : AttributeFsmImpl(rsize) {}

  /// It may be base class.
  virtual ~Trie() {}

  /**
   * \brief Adding chain of symbols.
   *
   * \param begin Iterator of the chain's begin.
   * \param end   Iterator of the chain's end.
   * \param id    Chain identifier.
   *
   * \return      The number of last state of the chain.
   */
  template <typename SymbolIterator>
  StateId AddChain(SymbolIterator begin, SymbolIterator end, const ChainId& id) {
    // Adding new chain.
    StateId state = AddChain(begin, end);

    // Adding attribute.
    AttributeFsmImpl::AddAttribute(state, id);

    return state;
  }

  /**
   * \brief Adding chain of symbols.
   *
   * \param begin Iterator of the chain's begin.
   * \param end   Iterator of the chain's end.
   *
   * \return      The number of last state of the chain.
   */
  template <typename SymbolIterator>
  StateId AddChain(SymbolIterator begin, SymbolIterator end) {
    // Go through the chain and add moves if any.
    StateId state = kStartState;
    for (SymbolIterator it = begin; it != end; ++it) {
      // Moving by the symbol.
      StateId state_to = AttributeFsmImpl::Go(state, *it);

      // If the move is absent then add it to FSM.
      if (state_to == kInvalidState) {
        state_to = AttributeFsmImpl::AddState();
        AttributeFsmImpl::AddTransition(state, state_to, *it);
      }

      state = state_to;
    }

    // Make last state acceptable.
    AttributeFsmImpl::MakeAcceptable(state);

    return state;
  }

  /**
   * \brief Search of the passed chain in the trie
   *
   * \param begin Iterator of the chain's begin.
   * \param end   Iterator of the chain's end.
   * \result      The reference to the list of attributes of the chain if any.
   */
  template <typename SymbolIterator>
  const typename AttributeFsmImpl::AttributeList& Search(SymbolIterator begin, SymbolIterator end) const {
    StateId state = kStartState;
    for (SymbolIterator it = begin; state !=  kInvalidState and it != end; ++it) {
      state = AttributeFsmImpl::Go(state, *it);
    }
    return AttributeFsmImpl::GetStateAttributes(state);
  }
};

}} // namespace strutext, automata.
