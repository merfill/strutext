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
 * \brief  NFA construction implementations.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <list>
#include <vector>
#include <set>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "nfa.h"

namespace strutext { namespace automata { namespace operations {

namespace utils {

template <typename SymbolCode, typename Attribute>
struct UnionResult {
  typedef Nfa<SymbolCode, Attribute>            NfaImpl;
  typedef std::vector<typename NfaImpl::State*> StateList;
  typedef std::set<typename NfaImpl::State*>    StateSet;
  typedef std::vector<StateSet>                 StateSetList;

  UnionResult() : automaton_(boost::make_shared<NfaImpl>()) {}

  typename NfaImpl::Ptr automaton_;
  StateList             start_states_;
  StateSetList          accepted_states_;
};

template <typename SymbolCode, typename Attribute>
UnionResult<SymbolCode, Attribute> CreateUnion(
    const std::vector<typename Nfa<SymbolCode, Attribute>::Ptr>& auto_list) {

  typedef Nfa<SymbolCode, Attribute>                            NfaImpl;
  typedef std::vector<typename Nfa<SymbolCode, Attribute>::Ptr> AutoList;
  typedef UnionResult<SymbolCode, Attribute>                    UnionResultImpl;

  UnionResultImpl result;

  typedef std::map<typename NfaImpl::State*, typename NfaImpl::State*> OldNewList;
  OldNewList old2new;

  // Firstly, for each state create its copy in result automaton.
  for (typename AutoList::const_iterator auto_it = auto_list.begin(); auto_it != auto_list.end(); ++auto_it) {
    result.start_states_.push_back(NULL);
    result.accepted_states_.push_back(typename UnionResultImpl::StateSet());

    // Go through the left NFA.
    for (typename NfaImpl::StateStorage::const_iterator st_it = (*auto_it)->states_.begin();
          st_it != (*auto_it)->states_.end(); ++st_it) {
      typename NfaImpl::State::Ptr state = boost::make_shared<typename NfaImpl::State>();
      result.automaton_->AddState(state);
      old2new[(*st_it).get()] = state.get();

      if ((*auto_it)->start_state_ == (*st_it).get()) {
        result.start_states_.back() = state.get();
      }

      if ((*auto_it)->accepted_states_.end() != (*auto_it)->accepted_states_.find((*st_it).get())) {
        result.accepted_states_.back().insert(state.get());
      }
    }
  }

  // Copy transition table.
  for (typename OldNewList::iterator st_it = old2new.begin(); st_it != old2new.end(); ++st_it) {
    for (typename NfaImpl::State::TransTable::const_iterator tr_it = st_it->first->trans_table_.begin();
          tr_it != st_it->first->trans_table_.end(); ++tr_it) {
      st_it->second->trans_table_.insert(typename NfaImpl::State::TransTable::value_type(
                                                                            tr_it->first, old2new[tr_it->second]));
    }
  }

  // Copy attributes.
  for (typename OldNewList::iterator st_it = old2new.begin(); st_it != old2new.end(); ++st_it) {
    st_it->second->attr_list_ = st_it->first->attr_list_;
  }

  return result;
}

/// Epsilon closure implementation for one state.
template <typename SymbolCode, typename Attribute>
void EpsilonClosure(typename Nfa<SymbolCode, Attribute>::State* state,
                      std::set<typename Nfa<SymbolCode, Attribute>::State*>& res) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  res.clear();
  res.insert(state);
  std::set<typename NfaImpl::State*> step_res = res;
  for (;;) {
    std::set<typename NfaImpl::State*> local_res;
    for (typename std::set<typename NfaImpl::State*>::iterator st_it = step_res.begin(); st_it != step_res.end(); ++st_it) {
      typename std::pair<typename NfaImpl::TransTable::iterator, typename NfaImpl::TransTable::iterator> ret =
                                                                (*st_it)->trans_table_.equal_range(NfaImpl::Symbol());
      for (typename NfaImpl::TransTable::iterator it = ret.first; it != ret.second; ++it) {
        if (res.find(it->second) == res.end()) {
          res.insert(it->second);
          local_res.insert(it->second);
        }
      }
    }
    if (local_res.empty()) {
      break;
    } else {
      local_res.swap(step_res);
    }
  }
}

/// Epsilon closure implementation for set of states.
template <typename SymbolCode, typename Attribute>
void EpsilonClosure(const std::set<typename Nfa<SymbolCode, Attribute>::State*>& input,
                      std::set<typename Nfa<SymbolCode, Attribute>::State*>& res) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  for (typename std::set<typename Nfa<SymbolCode, Attribute>::State*>::const_iterator st_it = input.begin();
        st_it != input.end(); ++st_it) {
    std::set<typename NfaImpl::State*> state_res;
    EpsilonClosure(*st_it, state_res);
    res.insert(state_res.begin(), state_res.end());
  }
}

/// Move on set of states.
template <typename SymbolCode, typename Attribute>
void Move(const std::set<typename Nfa<SymbolCode, Attribute>::State*>& input,
            const typename Nfa<SymbolCode, Attribute>::Symbol& symbol,
            std::set<typename Nfa<SymbolCode, Attribute>::State*>& res) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  for (typename std::set<typename Nfa<SymbolCode, Attribute>::State*>::const_iterator st_it = input.begin();
        st_it != input.end(); ++st_it) {
     typename std::pair<typename NfaImpl::TransTable::iterator, typename NfaImpl::TransTable::iterator> ret =
                                                                      (*st_it)->trans_table_.equal_range(symbol);
    std::set<typename NfaImpl::State*> state_res, closure_res;
    for (typename NfaImpl::TransTable::iterator it = ret.first; it != ret.second; ++it) {
      state_res.insert(it->second);
    }
    EpsilonClosure(state_res, closure_res);
    res.insert(closure_res.begin(), closure_res.end());
  }
}

/// Find move symbols for set of states.
template <typename SymbolCode, typename Attribute>
void FindSymbols(const std::set<typename Nfa<SymbolCode, Attribute>::State*>& input,
                  std::set<typename Nfa<SymbolCode, Attribute>::Symbol>& symbols) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  for (typename std::set<typename NfaImpl::State*>::const_iterator st_it = input.begin(); st_it != input.end(); ++st_it) {
    for (typename NfaImpl::TransTable::iterator it = (*st_it)->trans_table_.begin(); it != (*st_it)->trans_table_.end(); ++it) {
      if (it->first.type_ == NfaImpl::Symbol::SYMBOL_MT) {
        symbols.insert(it->first);
      }
    }
  }
}

/// Check does passed set of states contain accepted state.
template <typename SymbolCode, typename Attribute>
bool FindInAccepts(const std::set<typename Nfa<SymbolCode, Attribute>::State*>& input,
                    typename Nfa<SymbolCode, Attribute>::Ptr nfa) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  const std::set<typename NfaImpl::State*>& accepts = nfa->GetAcceptedStates();
  for (typename std::set<typename NfaImpl::State*>::const_iterator st_it = input.begin(); st_it != input.end(); ++st_it) {
    if (accepts.find(*st_it) != accepts.end()) {
      return true;
    }
  }
  return false;
}

/// Copy state attributes to new state.
template <typename SymbolCode, typename Attribute>
void CopyAttrs(const std::set<typename Nfa<SymbolCode, Attribute>::State*>& input,
                typename Nfa<SymbolCode, Attribute>::State* new_state) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  for (std::set<typename NfaImpl::State*> st_it = input.begin(); st_it != input.end(); ++st_it) {
    new_state->attr_list_.insert((*st_it)->attr_list_.begin(), (*st_it)->attr_list_.end());
  }
}

} // namespace utils.

template <typename SymbolCode, typename Attribute>
typename Nfa<SymbolCode, Attribute>::Ptr Concat(typename Nfa<SymbolCode, Attribute>::Ptr left,
                                                  typename Nfa<SymbolCode, Attribute>::Ptr right) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  std::vector<typename NfaImpl::Ptr> auto_list;
  auto_list.push_back(left);
  auto_list.push_back(right);
  utils::UnionResult<SymbolCode, Attribute> result = CreateUnion(auto_list);

  // Add transions form accepted states of left NFA to start state of right NFA.
  for (typename NfaImpl::StateSet::const_iterator st_it = result.accepted_states_[0].begin();
        st_it != result.accepted_states_[0].end(); ++st_it) {
    result.automaton_->AddEpsilonTransition(*st_it, result.start_states_[1]);
  }

  // Set start state.
  result.automaton_->SetStartState(result.start_states_[0]);

  // Set accepted states.
  for (typename std::set<typename NfaImpl::State*>::const_iterator st_it = result.accepted_states_[1].begin();
          st_it != result.accepted_states_[1].end(); ++st_it) {
    result.automaton_->AddToAcceptedSet(*st_it);
  }

  return result.automaton_;
}

template <typename SymbolCode, typename Attribute>
typename Nfa<SymbolCode, Attribute>::Ptr Union(typename Nfa<SymbolCode, Attribute>::Ptr left,
                                                typename Nfa<SymbolCode, Attribute>::Ptr right) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  std::vector<typename NfaImpl::Ptr> auto_list;
  auto_list.push_back(left);
  auto_list.push_back(right);
  utils::UnionResult<SymbolCode, Attribute> result = CreateUnion(auto_list);

  // Create new start state and add epsilon transitions from it to start states of operands.
  typename NfaImpl::State::Ptr start_state = boost::make_shared<typename NfaImpl::State>();
  result->AddState(start_state);
  result->start_state_ = start_state.get();
  result->AddEpsilonTransition(start_state.get(), result.start_states_[0]);
  result->AddEpsilonTransition(start_state.get(), result.start_states_[1]);

  // Set accepted states.
  for (typename std::set<typename NfaImpl::State*>::const_iterator st_it = result.accepted_states_[0].begin();
        st_it != result.accepted_states_[0].end(); ++st_it) {
    result.automaton_->AddToAcceptedSet(*st_it);
  }
  for (typename std::set<typename NfaImpl::State*>::const_iterator st_it = result.accepted_states_[1].begin();
        st_it != result.accepted_states_[1].end(); ++st_it) {
    result.automaton_->AddToAcceptedSet(*st_it);
  }

  return result.automaton_;
}

template <typename SymbolCode, typename Attribute>
typename Nfa<SymbolCode, Attribute>::Ptr Union(const std::list<typename Nfa<SymbolCode, Attribute>::Ptr>& auto_list) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  // Result NFA.
  typename NfaImpl::Ptr result = boost::make_shared<NfaImpl>();

  // Firstly, for each state create its copy.
  typedef std::map<typename NfaImpl::State*, typename NfaImpl::State*> OldNewlist;
  OldNewlist old2new;
  typename NfaImpl::State::Ptr start_state = boost::make_shared<typename NfaImpl::State>();
  result->AddState(start_state);
  result->SetStartState(start_state.get());

  // Copy states.
  for (typename std::list<typename NfaImpl::Ptr>::const_iterator auto_it = auto_list.begin();
        auto_it != auto_list.end(); ++auto_it) {
    NfaImpl* automaton = (*auto_it).get();
    for (typename NfaImpl::StateStorage::const_iterator st_it = automaton->states_.begin();
          st_it != automaton->states_.end(); ++st_it) {
      typename NfaImpl::State::Ptr state = boost::make_shared<typename NfaImpl::State>();
      result->AddState(state);
      old2new[(*st_it).get()] = state.get();

      // Add epsilon move from start result NFA's state to start state of added NFA.
      if (automaton->start_state_ == (*st_it).get()) {
        result->AddEpsilonTransition(start_state.get(), state.get());
      }

      // Make all added NFA's accepted states to be accepted in the result NFA.
      if (automaton->accepted_states_.end() != automaton->accepted_states_.find((*st_it).get())) {
        result->AddToAcceptedSet(state.get());
      }
    }
  }

  // Copy transition table.
  for (typename OldNewlist::iterator st_it = old2new.begin(); st_it != old2new.end(); ++st_it) {
    for (typename NfaImpl::State::TransTable::const_iterator tr_it = st_it->first->trans_table_.begin();
          tr_it != st_it->first->trans_table_.end(); ++tr_it) {
      st_it->second->trans_table_.insert(typename NfaImpl::State::TransTable::
                                            value_type(tr_it->first, old2new[tr_it->second]));
    }
  }

  // Copy state attributes.
  for (typename OldNewlist::iterator st_it = old2new.begin(); st_it != old2new.end(); ++st_it) {
    st_it->second->attr_list_ = st_it->first->attr_list_;
  }

  return result;
}

template <typename SymbolCode, typename Attribute>
typename Nfa<SymbolCode, Attribute>::Ptr Iteration(typename Nfa<SymbolCode, Attribute>::Ptr operand) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  // Create copy of the automaton operand.
  std::vector<typename NfaImpl::Ptr> auto_list;
  auto_list.push_back(operand);
  utils::UnionResult<SymbolCode, Attribute> result = CreateUnion(auto_list);

  // Create start state and add move from it to start state of operand automaton.
  typename NfaImpl::State::Ptr start_state = boost::make_shared<typename NfaImpl::State>();
  result.automaton_->AddState(start_state);
  result.automaton_->SetStartState(start_state.get());
  result.automaton_->AddEpsilonTransition(start_state.get(), result.start_states_[0]);

  // Create new accepted state.
  typename NfaImpl::State::Ptr accept_state = boost::make_shared<typename NfaImpl::State>();
  result.automaton_->AddState(accept_state);
  result.automaton_->AddToAcceptedSet(accept_state.get());

  // Add transition from start state to the accepted state.
  result.automaton_->AddEpsilonTransition(start_state.get(), accept_state.get());

  // Add transition from each accepted state of operand automaton to new accepted state
  // and to its start state.
  for (typename std::set<typename NfaImpl::State*>::const_iterator st_it = result.accepted_states_[0].begin();
        st_it != result.accepted_states_[0].end(); ++st_it) {
    result.automaton_->AddEpsilonTransition(*st_it, result.start_states_[0]);
    result.automaton_->AddEpsilonTransition(*st_it, accept_state.get());
  }

  return result.automaton_;
}

template <typename SymbolCode, typename Attribute>
typename Nfa<SymbolCode, Attribute>::Ptr Copy(typename Nfa<SymbolCode, Attribute>::Ptr operand) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  // Create copy of the automaton operand.
  std::vector<typename NfaImpl::Ptr> auto_list;
  auto_list.push_back(operand);
  utils::UnionResult<SymbolCode, Attribute> result = CreateUnion(auto_list);

  // Set start state.
  result.automaton_->SetStartState(result.start_states_[0]);

  // Set accepted states.
  for (typename std::set<typename NfaImpl::State*>::const_iterator st_it = result.accepted_states_[0].begin();
        st_it != result.accepted_states_[0].end(); ++st_it) {
    result.automaton_->AddToAcceptedSet(*st_it);
  }

  return result.automaton_;
}

template <typename SymbolCode, typename Attribute>
typename Nfa<SymbolCode, Attribute>::Ptr Invert(typename Nfa<SymbolCode, Attribute>::Ptr automaton) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  // Result NFA.
  typename NfaImpl::Ptr result = boost::make_shared<NfaImpl>();

  typedef std::map<typename NfaImpl::State*, typename NfaImpl::State*> Old2NewList;
  Old2NewList old2new;

  // New state as invertion of all accepted states in old automaton.
  typename NfaImpl::State::Ptr start_state = boost::make_shared<typename NfaImpl::State>();
  result->AddState(start_state);
  result->SetStartState(start_state.get());

  // Go through the operand's states.
  for (typename NfaImpl::StateStorage::const_iterator st_it = automaton->states_.begin();
        st_it != automaton->states_.end(); ++st_it) {
    typename NfaImpl::State::Ptr state = boost::make_shared<typename NfaImpl::State>();
    result->AddState(state);
    old2new[(*st_it).get()] = state.get();

    if (automaton->GetStartState() == (*st_it).get()) {
      result->AddToAcceptedSet(state.get());
    }

    if (automaton->accepted_states_.end() != automaton->accepted_states_.find((*st_it).get())) {
      result->AddEpsilonTransition(result->start_state_, state.get());
    }
  }

  // Add accepted state attributes to new accepted state.
  for (typename NfaImpl::StateSet::const_iterator st_it = automaton->accepted_states_.begin();
        st_it != automaton->accepted_states_.end(); ++st_it) {
    for (typename NfaImpl::StateSet::iterator  it = result->accepted_states_.begin();
          it != result->accepted_states_.end(); ++it) {
      (*it)->attr_list_.insert((*st_it)->attr_list_.begin(), (*st_it)->attr_list_.end());
    }
  }

  // Invert transitions.
  for (typename Old2NewList::iterator st_it = old2new.begin(); st_it != old2new.end(); ++st_it) {
    for (typename NfaImpl::State::TransTable::const_iterator tr_it = st_it->first->trans_table_.begin();
          tr_it != st_it->first->trans_table_.end(); ++tr_it) {
      result->AddTransition(old2new[tr_it->second], old2new[st_it->first], tr_it->first);
    }
  }

  return result;
}

template <typename SymbolCode, typename Attribute>
typename Nfa<SymbolCode, Attribute>::Ptr TransformToDfa(typename Nfa<SymbolCode, Attribute>::Ptr automaton) {
  typedef Nfa<SymbolCode, Attribute> NfaImpl;

  // Create DFA.
  typename NfaImpl::Ptr dfa = boost::make_shared<NfaImpl>();
  if (automaton->GetStates().size() == 0) {
    return dfa;
  }

  // Create DFA's start state as the set of states consisting of NFA's start state.
  typename NfaImpl::State::Ptr start_state = boost::make_shared<typename NfaImpl::State>();
  dfa->AddState(start_state);
  dfa->SetStartState(start_state.get());

  // Fill start state's set.
  std::set<typename NfaImpl::State*> start_state_set;
  utils::EpsilonClosure(automaton->GetStartState(), start_state_set);
  if (utils::FindInAccepts(start_state_set, automaton)) {
    dfa->AddToAcceptedSet(start_state.get());
  }

  // Releation connects sets of NFA's states and DFA's states.
  typedef boost::hash<std::set<typename NfaImpl::State*> > Hash;
  typedef std::map<std::set<typename NfaImpl::State*>, typename NfaImpl::State*> DfaStateSet;
  typedef std::map<size_t, typename NfaImpl::State*> DfaHashStateSet;
  DfaHashStateSet dfa_states;
  Hash start_state_hash;
  dfa_states[start_state_hash(start_state_set)] = start_state.get();

  // Current step state set.
  DfaStateSet step_set;
  step_set[start_state_set] = start_state.get();

  // Iterate while at least one state needed to be handled.
  for (;;) {
    // One iteration state set.
    DfaStateSet local_set;
    for (typename DfaStateSet::iterator set_it = step_set.begin(); set_it != step_set.end(); ++set_it) {
      // Find symbols with transitions.
      std::set<typename NfaImpl::Symbol> symbols;
      utils::FindSymbols(set_it->first, symbols);

      // Go through symbols and create states if any.
      for (typename std::set<typename NfaImpl::Symbol>::iterator sym_it = symbols.begin(); sym_it != symbols.end(); ++sym_it) {
        // Create transition.
        std::set<typename NfaImpl::State*> move_set;
        utils::Move(set_it->first, *sym_it, move_set);

        // Create the state if any.
        typename NfaImpl::State* state = NULL;
        Hash hash;
        const size_t move_set_hash = hash(move_set);
        typename DfaHashStateSet::iterator mv_it = dfa_states.find(move_set_hash);
        if (mv_it == dfa_states.end()) {
          typename NfaImpl::State::Ptr new_state = boost::make_shared<typename NfaImpl::State>();
          dfa->AddState(new_state);
          state = new_state.get();
          utils::CopyAttrs(move_set, new_state.get());
          if (utils::FindInAccepts(move_set, automaton)) {
            dfa->AddToAcceptedSet(state);
          }
          dfa_states[move_set_hash] = state;
          local_set[move_set] = state;
        } else {
          state = mv_it->second;
        }
        dfa->AddTransition(set_it->second, state, *sym_it);
      }
    }

    // Check at least one state is bult and break the loop if it is not so.
    if (local_set.empty()) {
      break;
    } else {
      step_set.swap(local_set);
    }
  }

  return dfa;
}

}}} // namespace strutext, automata, operations.
