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
 * \brief  NFA implementation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <cstddef>
#include <cassert>
#include <set>
#include <map>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace strutext { namespace automata {

/**
 * \brief Nondeterministic Finite Nfa (NFA) implementation.
 *
 * NFA may implement more then one transition by the same symbol. There are also transitions
 * by empty (epsilon) symbol.
 */
template <typename SymbolCode, typename Attribute>
struct Nfa : private boost::noncopyable {
  /// Smart pointer type.
  typedef boost::shared_ptr<Nfa> Ptr;

  /// Symbol type.
  struct Symbol {
    /// Symbol kind.
    enum Type {
      SYMBOL_MT    ///< Ordinar symbol.
      , EPSILON_MT ///< Epsilon symbol.
    };

    /// Default initialization by epsilon move.
    Symbol()
      : type_(EPSILON_MT)
      , symbol_() {}

    /// Initialization by symbol move.
    explicit Symbol(SymbolCode symbol)
      : type_(SYMBOL_MT)
      , symbol_(symbol) {}

    bool operator<(const Symbol& other) const {
      if (type_ == EPSILON_MT and other.type_ == EPSILON_MT) {
        return false;
      } else if (type_ == EPSILON_MT and other.type_ == SYMBOL_MT) {
        return true;
      } else if (type_ == SYMBOL_MT and other.type_ == EPSILON_MT) {
        return false;
      }
      return symbol_ < other.symbol_;
    }

    bool operator==(const Symbol& other) const {
      if (type_ == EPSILON_MT and other.type_ == EPSILON_MT) {
        return true;
      }
      if (type_ == SYMBOL_MT and other.type_ == SYMBOL_MT) {
        return symbol_ == other.symbol_;
      }
      return false;
    }

    Type       type_;   ///< Transition kind.
    SymbolCode symbol_; ///< Symbol code -- only for ordinar symbols.
  };

  /// NFA state implementation.
  struct State {
    typedef boost::shared_ptr<State> Ptr;

    /// Symbol table type.
    typedef std::multimap<Symbol, State*> TransTable;

    /// Attribute set type.
    typedef std::set<Attribute> AttrList;

    TransTable trans_table_; ///< Transition table for the state.
    AttrList   attr_list_;   ///< State attributes.
  };

  typedef std::set<typename State::Ptr> StateStorage;
  typedef std::set<State*>     StateSet;

  Nfa() : start_state_(NULL) {}

  /// Adding new state to the NFA.
  void AddState(typename State::Ptr state) {
    states_.insert(state);
  }

  /**
   * \brief Add transition.
   *
   * \param from   Domain state.
   * \param to     Codomain state.
   * \param symbol Transition symbol.
   */
  void AddTransition(State* from, State* to, SymbolCode symbol) {
    assert(from and "state must be not NULL");
    assert(to and "state must be not NULL");
    from->trans_table_.insert(State::TransTable::value_type(Symbol(symbol), to));
  }

  /**
   * \brief Add transition.
   *
   * \param from   Domain state.
   * \param to     Codomain state.
   * \param symbol Transition symbol.
   */
  void AddTransition(State* from, State* to, Symbol symbol) {
    assert(from and "state must be not NULL");
    assert(to and "state must be not NULL");
    from->trans_table_.insert(State::TransTable::value_type(symbol, to));
  }

  /**
   * \brief Add epsilon transition.
   *
   * \param from   Domain state.
   * \param to     Codomain state.
   */
  void AddEpsilonTransition(State* from, State* to) {
    assert(from and "state must be not NULL");
    assert(to and "state must be not NULL");
    from->trans_table_.insert(State::TransTable::value_type(Symbol(), to));
  }

  /**
   * \brief Add state to accepted ones.
   *
   * \param state State to add.
   */
  void AddToAcceptedSet(State* state) {
    assert(state and "state must be not NULL");
    accepted_states_.insert(state);
  }

  /// Set start state.
  void SetStartState(State* state) {
    assert(state and "state must be not NULL");
    start_state_ = state;
  }

  /// Add attribute to each of accepted states.
  void AddAttrToAcceptedStates(const Attribute& attr) {
    for (typename StateSet::iterator it = accepted_states_.begin(); it != accepted_states_.end(); ++it) {
      (*it)->attr_list_.insert(attr);
    }
  }

  /// Get start state.
  State* GetStartState() const {
    return start_state_;
  }

  /// Get accepted state set.
  const StateSet& GetAcceptedStates() const {
    return accepted_states_;
  }

  /// Get NFA's state set.
  const StateStorage& GetStates() const {
    return states_;
  }

  State*       start_state_;     ///< Start state pointer.
  StateStorage states_;          ///< All NFA's state set.
  StateSet     accepted_states_; ///< Accepted state set.
};

}} // namespace strutext, automata.
