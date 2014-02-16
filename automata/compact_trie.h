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

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

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
    Print(trie);
    Minimize(trie);
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
  template <class Iterator>
  AttributeList Match(Iterator begin, Iterator end) {
    AttributeList result;
    StateId state = kStartState;
    for (Iterator it = begin; it != end; ++it) {
      StateId prev_state = state;
      const SymbolType& symbol = *it;
      state = FsmImpl::Go(state, symbol);
      if (state == kInvalidState) {
        break;
      } else if (FsmImpl::IsAcceptable(prev_state)) { // Clear attribute list if go through accptable state.
        result = AttributeList();
      }

      // Search state attributes.
      typename StateAttributeList::iterator state_it = attributes_.find(state);
      if (state_it != attributes_.end()) {
        // Search move attributes.
        const MoveAttributeList& state_attributes = state_it->second;
        typename MoveAttributeList::iterator  move_it = state_attributes.find(symbol);
        if (move_it != state_attributes.end()) {
          const AttributeVector& move_attrs = move_it->second;
          result = move_attrs;
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
  // ****** Some internal types and functions to minimize input trie *****

  // State information structure to build minimized trie.
  struct StateInfo {
    typedef boost::unordered_map<SymbolType, StateInfo*>    MoveList;
    typedef std::list<AttributeType>                        AttributeList;
    typedef boost::unordered_map<SymbolType, AttributeList> MoveAttributeList;

    StateInfo(StateId state_id, SymbolType in_symbol, bool is_acceptable)
      : state_id_(state_id)
      , in_symbol_(in_symbol)
      , is_acceptable_(is_acceptable) {}

    StateId           state_id_;
    SymbolType        in_symbol_;
    bool              is_acceptable_;
    MoveList          move_list_;
    MoveAttributeList attr_list_;
  };

  // State equality implementation.
  struct StateInfoEqual : std::binary_function<StateInfo, StateInfo, bool> {
    bool operator()(const StateInfo& left, const StateInfo& right) const {
      if (left.is_acceptable_ != right.is_acceptable_) {
        return false;
      }
      if (left.in_symbol_ != right.in_symbol_) {
        return false;
      }
      if (left.move_list_.size() != right.move_list_.size()) {
        return false;
      }
      for (typename StateInfo::MoveList::const_iterator lit = left.move_list_.begin(), rit = right.move_list_.end();
              lit != left.move_list_.end() and rit != right.move_list_.end(); ++lit, ++rit) {
        if (lit->first != rit->first or lit->second != rit->second) {
          return false;
        }
      }
      return true;
    }
  };

  // State hash builder implementation.
  struct StateInfoHash : std::unary_function<StateInfo, size_t> {
    size_t operator()(const StateInfo& state) const {
      size_t seed = 0;
      boost::hash_combine(seed, state.is_acceptable_);
      boost::hash_combine(seed, state.in_symbol_);
      for (typename StateInfo::MoveList::const_iterator it = state.move_list_.begin(); it != state.move_list_.end(); ++it) {
        boost::hash_combine(seed, it->first);
        boost::hash_combine(seed, it->second);
      }
      return seed;
    }
  };

  typedef boost::shared_ptr<StateInfo>                                                 StateInfoPtr;
  typedef boost::unordered_map<StateInfo, StateInfoPtr, StateInfoHash, StateInfoEqual> StateInfoSet;
  typedef std::pair<StateInfo*, typename StateInfo::AttributeList>                     MinimizationResult;

  // Tri minimization recursive method.
  static MinimizationResult Minimize(StateId state_id, SymbolType symbol, const TrieImpl& trie, StateInfoSet& reg) {
    typename StateInfo::AttributeList attrs;
    StateInfoPtr state = boost::make_shared<StateInfo>(state_id, symbol, trie.IsAcceptable(state_id));

    // Go through move list of the state and go down for each transition.
    typename TransitionType::TransTable mt = trie.GetMoveTable(state_id);
    for (typename TransitionType::TransTable::iterator mt_it = mt.begin(); mt_it != mt.end(); ++mt_it) {
      // Go down the transion.
      MinimizationResult result = Minimize(mt_it->second, mt_it->first, trie, reg);

      // Add transiotion to the new automation.
      StateInfo* next_state = result.first;
      state->move_list_.insert(std::make_pair(mt_it->first, next_state));

      // Pass attribute list up or save it in the current transion.
      if ((state->is_acceptable_ or mt.size() > 1) and not result.second.empty()) {
        state->attr_list_.insert(std::make_pair(mt_it->first, result.second));
      } else {
        attrs = result.second;
      }
    }

    // Search the state in the storage.
    typename StateInfoSet::iterator it = reg.find(*state);
    if (it != reg.end()) {
      // Do not create state as it is already created.
      state = it->second;
    } else {
      // Register and save the state in the storage.
      reg.insert(std::make_pair(*state, state));
    }

    // We need to inherit attributes of acceptable state.
    if (state->is_acceptable_) {
      attrs = typename StateInfo::AttributeList(trie.states_attr_[state_id].begin(), trie.states_attr_[state_id].end());
    }

    return MinimizationResult(state.get(), attrs);
  }

  void Minimize(const TrieImpl& trie) {
    StateInfoSet reg;
    MinimizationResult result = Minimize(kStartState, 0, trie, reg);
    StateInfo* start_state = result.first;
    std::cout << "\n\n";
    Print(start_state);
  }

  void Print(const StateInfo* start_state) {
    typedef boost::unordered_set<const StateInfo*> StateList;
    typedef boost::unordered_map<const StateInfo*, StateId> StateIdList;

    StateList st_list;
    StateIdList handled_states;
    st_list.insert(start_state);
    while (true) {
      StateList new_states;
      for (typename StateList::iterator st_it = st_list.begin(); st_it != st_list.end(); ++st_it) {
        if (handled_states.find(*st_it) != handled_states.end()) {
          continue;
        }

        StateId cur_state_id = kStartState;
        if (*st_it != start_state) {
          cur_state_id = FsmImpl::AddState((*st_it)->is_acceptable_);
        }
        typename StateInfo::MoveList mv = (*st_it)->move_list_;
        for (typename StateInfo::MoveList::const_iterator mv_it = mv.begin(); mv_it != mv.end(); ++mv_it) {
          FsmImpl::AddTransition(cur_state_id);
          std::cout << " (" << mv_it->first << ";" << mv_it->second->state_id_ << ")";
          std::cout << " => [";
          typename StateInfo::MoveAttributeList::const_iterator attr_it = (*st_it)->attr_list_.find(mv_it->first);
          if (attr_it != (*st_it)->attr_list_.end()) {
            const typename StateInfo::AttributeList& attrs = attr_it->second;
            for (typename StateInfo::AttributeList::const_iterator ait = attrs.begin(); ait != attrs.end(); ++ait) {
              std::cout << " " << *ait;
            }
          }
          std::cout << "]";
          new_states.insert(mv_it->second);
        }
        std::cout << "\n";
      }
      if (new_states.empty()) {
        break;
      } else {
        st_list = new_states;
      }
    }
  }

  void Print(const TrieImpl& trie) {
    typedef std::set<StateId> StateList;

    StateList st_list;
    st_list.insert(kStartState);
    while (true) {
      StateList new_states;
      for (StateList::iterator st_it = st_list.begin(); st_it != st_list.end(); ++st_it) {
        if (*st_it == kStartState) {
            std::cout << "--> ";
        }
        if (trie.IsAcceptable(*st_it)) {
          std::cout << " *  ";
        }
        std::cout << *st_it << "  ";
        typename TransitionType::TransTable mt = trie.GetMoveTable(*st_it);
        for (typename TransitionType::TransTable::iterator mt_it = mt.begin(); mt_it != mt.end(); ++mt_it) {
          std::cout << " (" << mt_it->first << ";" << mt_it->second << ")";
          new_states.insert(mt_it->second);
        }
        std::cout << "\n";
      }
      if (new_states.empty()) {
        break;
      } else {
        st_list = new_states;
      }
    }
  }

  StateAttributeList attributes_;
};

}} // namespace strutext, automata.
