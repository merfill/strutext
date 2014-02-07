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
 * \brief  Finite State Machine definition.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <cstddef>
#include <vector>

#include "fsm_defs.h"

namespace strutext { namespace automata {

/// FSM Serializer declaration.
template <class TransImpl> struct FsmSerializer;

/**
 * \brief Finite State Machine implementation.
 *
 * Finite State Machine has a set of states. There are moves from
 * the states, that can be done by reading symbols. There is start
 * state and set of finite states.
 * The automaton gets moving table implementation as template parameter.
 */
template <typename TransImpl>
struct FiniteStateMachine {
  /// Type of transition table.
  typedef TransImpl                       Transitions;

  /// Symbol type.
  typedef typename Transitions::CharType  CharType;

  /// Number of reserved states by default.
  static const size_t kReservedStateTableSize = 10000u;

  /// State definition.
  struct State {
    Transitions trans_;       ///< Move table.
    bool        is_accepted_; ///< Is the state accepptable.

    /// Default initialization.
    explicit State(bool is_accepted = false) : is_accepted_(is_accepted) {}
  };

  /// Type of states' list.
  typedef std::vector<State> StateTable;

  // Make serializer class as friend.
  friend struct FsmSerializer<TransImpl>;

  /**
   * \brief Default initialization.
   *
   * \param st_size The rezerved size of the table of states.
   */
  explicit FiniteStateMachine(size_t st_size = kReservedStateTableSize) {
    // Reserve passed number of states.
    states_.reserve(st_size);

    // Any automaton has at least two states:

    // 1. Incorrect state with index 0.
    states_.push_back(State());

    // 2. Start state with index 1.
    states_.push_back(State());
  }

  /// The class may be inherited.
  virtual ~FiniteStateMachine() {}

  /// Return number of states of the automata.
  size_t GetNumOfStates() const { return states_.size(); }

  /**
   * \brief Transiotion by symbol implementation.
   *
   * \param  state  The state to move from.
   * \param  symbol The move symbol.
   * \return        Move state number or kInvalidState if move is absent.
   */
  StateId Go(const StateId& state, const CharType& symbol) const { return states_[state].trans_.Go(symbol); }

  /**
   * \brief Adding new state.
   *
   * \param   is_accepted If it is true, then added state will be acceptable.
   * \return              The new state index.
   */
  StateId AddState(bool is_accepted = false) {
    states_.push_back(State(is_accepted));
    return states_.size() - 1;
  }

  /**
   * \brief Adding new move.
   *
   * \param from    State to move from.
   * \param to      State to move to.
   * \param symbol  The move's symbol.
   */
  void AddTransition(const StateId& from, const StateId& to, const CharType& symbol) {
    states_[from].trans_.AddTransition(to, symbol);
  }

  /**
   * \brief Making the state to be acceptable.
   *
   * \param state The state number.
   */
  void MakeAcceptable(const StateId& state) {
    states_[state].is_accepted_ = true;
  }

  /**
   * \brief Is the state acceptable?
   *
   * \param  state  The state number.
   * \return        true if the state is acceptable and false otherwise.
   */
  bool IsAcceptable(const StateId& state) const {
    return states_[state].is_accepted_;
  }

  /// Return reference to the move table.
  typename Transitions::TransTable GetMoveTable(const StateId& state) const {
      return states_[state].trans_.GetMoveTable();
  }

  StateTable states_; ///< The table of states.
};

}} // namespace strutext, automata.
