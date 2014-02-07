/** Copyright &copy; 2013-2014, Vladimir Lapshin.
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
 * \brief  Compact trie imlementation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <vector>
#include <list>
#include <set>
#include <map>

#include "trie.h"

namespace strutext { namespace automata {

/**
 * \brief Compact trie class implementation.
 *
 * Compact trie contains set of symbol chains (words) -- dictionary. In contrast to ordinary trie,
 * which has treelike structure, compact trie has acyclic graph structure. All words that have the
 * same suffixes have the same state/move sequences for the suffixes.
 * An each word can have list of attributes. In the implementation attributes attached to
 * transitions.
 * T1 -- transition implementation type.
 * T2 -- attribute type.
 */
template <class T1, typename T2>
struct CompactTrie : public FiniteStateMachine<T1> {
  /// Type of transition implementation.
  typedef T1 TransitionType;

  /// Type of the attribute.
  typedef T2 AttributeType;

  /// FSM implementation type.
  typedef FiniteStateMachine<TransitionType> FsmImpl;

  /// Symbol type.
  typedef typename FsmImpl::CharType SymbolType;

  /// Attribute vector type.
  typedef std::vector<AttributeType> AttributeVector;

  /// Attribute list type.
  typedef std::list<AttributeType> AttributeList;

  /// State attribute list type.
  typedef std::map<SymbolType, AttributeVector> MoveAttributeList;

  /// Attribute list for the automaton.
  typedef std::map<StateId, MoveAttributeList> StateAttributeList;

  /// Trie type for the implementation.
  typedef Trie<TransitionType, AttributeType> TrieImpl;

  /**
   * \brief Initialization from the trie.
   *
   * The compact trie is built from the classical trie, which passed as initialization parameter.
   */
  explicit CompactTrie(const TrieImpl& trie) {
    BuildCompactTrie(trie);
  }

  /**
   * \brief Search passed chain in the trie.
   *
   * T -- chain iterator type.
   *
   * \param begin Begin iterator of the chain.
   * \param end   End iterator of the chain.
   * \return      List of the chain's attributes if any.
   */
  template <class T>
  AttributeList Match(T begin, T end) {
    AttributeList result;
    StateId state = kStartState;
    for (T it = begin; it != end; ++it) {
      const SymbolType& symbol = *it;
      state = FsmImpl::Go(state, symbol);
      if (state == kInvalidState) {
        break;
      } else {
        // Search state attributes.
        typename StateAttributeList::iterator state_it = attributes_.find(state);
        if (state_it != attributes_.end()) {
          const MoveAttributeList& state_attributes = state_it->second;
          // Search move attributes.
          typename MoveAttributeList::iterator  move_it = state_attributes.find(symbol);
          if (move_it != state_attributes.end()) {
            const AttributeVector& move_attrs = move_it->second;
            result.insert(result.end(), move_attrs.begin(), move_attrs.end());
          }
        }
      }
    }

    // If state is acceptable, then return accumulated attribute list.
    if (FsmImpl::IsAcceptable(state)) {
      return result;
    }

    return AttributeList();
  }

private:
  /// Build compact trie from the trie.
  void BuildCompactTrie(const TrieImpl& trie) {
    typedef std::pair<StateId, SymbolType> Move;
    typedef std::map<StateId, Move> StateWithMoveList;
    typedef std::set<StateId> StateList;

    // Collect state moves. For each state there is only one move to this. Collect triples
    // (state, state_from, symbol).
    StateWithMoveList st_moves;
    StateList st_list;
    st_list.insert(kStartState);
    while (true) {
      StateList new_states;
      for (StateList::iterator st_it = st_list.begin(); st_it != st_list.end(); ++st_it) {
        typename TransitionType::TransTable mt = trie.GetMoveTable(*st_it);
        for (typename TransitionType::TransTable::iterator mt_it = mt.begin(); mt_it != mt.end(); ++mt_it) {
          st_moves.insert(std::make_pair(mt_it->second, Move(*st_it, mt_it->first)));
          new_states.insert(mt_it->second);
        }
      }
      if (new_states.empty()) {
        break;
      } else {
        st_list = new_states;
      }
    }

    // Now, paste together states with the same moves to.

    typedef 

    // At first, glue acceptable states.



    for (typename StateWithMoveList::iterator it = st_moves.begin(); it != st_moves.end(); ++it) {
      std::cout << it->first << " <- (" << it->second.first << ";" << it->second.second << ")\n";
    }

  }

  StateAttributeList attributes_;
};

}} // namespace strutext, automata.
