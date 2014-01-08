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
 * \brief  NFA print.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <cstddef>
#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <queue>

#include "nfa.h"

namespace strutext { namespace automata {

/// NFA print to the passed stream.
template <typename SymbolCode, typename Attribute>
struct NfaPrinter {
  /// NFA type.
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  static void Print(typename NfaImpl::Ptr automaton, std::ostream& st) {
    // ***** At first, encoint the automaton states.

    // The enconting order if from start state and going through the moves.
    size_t st_num = 0;
    std::map<size_t, typename NfaImpl::State*> id2st_map;
    std::map<typename NfaImpl::State*, size_t> st2id_map;
    std::set<typename NfaImpl::State*> handled_set;
    std::queue<typename NfaImpl::State*> current_state_list;

    current_state_list.push(automaton->GetStartState());
    while (not current_state_list.empty()) {
      typename NfaImpl::State* state = current_state_list.front();
      current_state_list.pop();

      // Check the stated has been already handled.
      if (handled_set.find(state) != handled_set.end()) {
        continue;
      }

      // Get state Id.
      handled_set.insert(state);
      id2st_map[++st_num] = state;
      st2id_map[state] = st_num;

      // Go through move table and add all move states to be handled.
      for (typename NfaImpl::State::TransTable::const_iterator trans_it = state->trans_table_.begin();
                                                  trans_it != state->trans_table_.end(); ++trans_it) {
        current_state_list.push(trans_it->second);
      }
    }

    // ***** Now print encounted states.

    for (typename std::map<size_t, typename NfaImpl::State*>::const_iterator st_it = id2st_map.begin();
          st_it != id2st_map.end(); ++st_it) {
      typename NfaImpl::State* state = st_it->second;

      // Print start (accepted) state info if any.
      if (state == automaton->start_state_) {
        st << "--> ";
      }
      if (automaton->accepted_states_.find(state) != automaton->accepted_states_.end()) {
        st << "  * ";
      } else {
        st << "  ";
      }

      // Print state id.
      st << st_it->first;

      // Print transitions.
      for (typename NfaImpl::State::TransTable::const_iterator trans_it = state->trans_table_.begin();
                                                  trans_it != state->trans_table_.end(); ++trans_it) {
        st << " (";
        std::stringstream num;
        if (trans_it->first.type_ == NfaImpl::Symbol::SYMBOL_MT) {
          num << trans_it->first.symbol_; //"x" << std::hex << static_cast<size_t>(trans_it->first.symbol_);
        } else if (trans_it->first.type_ == NfaImpl::Symbol::EPSILON_MT) {
          num << "epsilon";
        }
        st <<  num.str() << ";" << st2id_map[trans_it->second] << ")";
      }
      st << "\n";
    }
  }
};

}}  // namespace strutext, automata.
