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
 * \brief  Aho Corasick trie imlementation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <list>
#include <vector>
#include <algorithm>

#include "trie.h"

namespace strutext { namespace automata {

/**
 * \brief Aho-Corasick trie implementation.
 */
template <class TransImpl, typename Attribute>
struct AhoCorasickTrie : public Trie<TransImpl, Attribute> {
  /// Move table type.
  typedef TransImpl Transitions;

  /// Trie type.
  typedef Trie<TransImpl, Attribute> TrieImpl;

  /// Fail transitions set type.
  typedef std::vector<StateId> FailMoveList;

  FailMoveList fail_moves_; ///< Fail transitions set.
};

/**
 * \brief Fail moves generator definition.
 */
template <class AcTrie>
class FailMoveGenerator {
public:
  /**
   * \brief Fail transitions generation.
   *
   * \param[out] ac_trie Trie to fill by fail transitions.
   */
  static void Generate(AcTrie& ac_trie) {
    // Prepare the buffer for fail moves.
    ac_trie.fail_moves_.resize(ac_trie.GetNumOfStates());

    // Type of state id set.
    typedef std::list<StateId> StateIdSet;

    // Type of move table.
    typedef typename AcTrie::Transitions::TransTable TransTableImpl;

    // Work states.
    StateIdSet work_states;

    // Fail move for start state.
    ac_trie.fail_moves_[kStartState] = automata::kStartState;

    // Set fail move to the start state.
    const TransTableImpl& st_moves = ac_trie.GetMoveTable(kStartState);
    for (typename TransTableImpl::const_iterator move_it = st_moves.begin(); move_it != st_moves.end(); ++move_it) {
      ac_trie.fail_moves_[move_it->second] = kStartState;
      work_states.push_front(move_it->second); // Don't forget to add move to the queue.
    }

    // Do while work states set is not empty.
    while (not work_states.empty()) {
      // Select the state from the queue.
      StateId state = work_states.back();
      work_states.pop_back();

      // Go throgh the state's moves.
      const TransTableImpl& st_moves = ac_trie.GetMoveTable(state);
      for (typename TransTableImpl::const_iterator move_it = st_moves.begin(); move_it != st_moves.end(); ++move_it) {
        // Add moving state to work states set.
        work_states.push_front(move_it->second);

        // Down through fail moves while go to the start state or there is move by the symbol.
        StateId fstate = ac_trie.fail_moves_[state];
        while (fstate != kStartState) {
          // Do move.
          StateId st = ac_trie.Go(fstate, move_it->first);

          // If there is fail move from the state than "continue" it.
          if (st != kInvalidState) {
            break;
          }
          fstate = ac_trie.fail_moves_[fstate];
        }

        // Add faile move to non start state only if there is move by the current symbol.
        StateId st = ac_trie.Go(fstate, move_it->first);
        if (st != kInvalidState) {
          ac_trie.fail_moves_[move_it->second] = st;
        } else {
          ac_trie.fail_moves_[move_it->second] = kStartState;
        }
      }
    }

    // Process including chains.
    for (StateId i = kStartState; i < ac_trie.GetNumOfStates(); ++i) {
      StateId state = i;
      StateId fstate = ac_trie.fail_moves_[state];
      while (fstate != kStartState) {
        // Add the chain index to the attribute list if the state is acceptable.
        if (ac_trie.IsAcceptable(fstate)) {
          const typename AcTrie::AttributeList& chain_ids = ac_trie.GetStateAttributes(fstate);
          // Go through identifiers.
          for (typename AcTrie::AttributeList::const_iterator chain_it = chain_ids.begin(); chain_it != chain_ids.end(); ++chain_it) {
            // Search the identifier in the attribute list and add if the identifier hasn't been found.
            const typename AcTrie::AttributeList& ids = ac_trie.GetStateAttributes(state);
            if (std::find(ids.begin(), ids.end(), *chain_it) == ids.end()) {
              ac_trie.AddAttribute(state, *chain_it);
            }
          }
        }
        // Go the next fail transition.
        fstate = ac_trie.fail_moves_[fstate];
      }
    }
  }
};

/**
 * \brief Aho-Corasick mover.
 */
template <class AcTrie>
class AcProcessor {
public:
  /// Initialization by AC trie object.
  explicit AcProcessor(const AcTrie& ac_trie) : ac_trie_(ac_trie) {}

  /**
   * \brief Move implementation.
   *
   * \param  from State to move from.
   * \param  sym  Symbol to move.
   * \return      State to move to.
   */
  StateId Move(const StateId& from, const typename AcTrie::CharType& sym) const {
    // Go by the symbol.
    StateId to = ac_trie_.Go(from, sym);

    // If the move is absent, then try to mive by fail transitions.
    StateId cs = from;
    while (to == kInvalidState and cs > kStartState) {
      cs = ac_trie_.fail_moves_[cs];
      to = ac_trie_.Go(cs, sym);
    }

    // Special check for the start state.
    if (to == kInvalidState) {
      to =  kStartState;
    }

    return to;
  }

private:
  const AcTrie& ac_trie_; ///< Reference to AC trie.
};

}} // namespace strutext, automata.
