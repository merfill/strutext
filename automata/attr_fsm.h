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
 * \brief  Attribute FSM implementation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <vector>

#include "fsm.h"

namespace strutext { namespace automata {

/// Serializer forward declaration.
template <class> struct AttrFsmSerializer;

/**
 * \brief FSM with attributes definition.
 *
 * State attribute is some template type. An each state contains a list of such attributes.
 */
template <class T1, typename T2>
struct AttributeFsm : public FiniteStateMachine<T1> {
  /// Type of transition table.
  typedef T1 Transition;

  /// Type of the attribute.
  typedef T2 Attribute;

  /// FSM implementation type.
  typedef FiniteStateMachine<Transition> FsmImpl;

  /// Attribute list type.
  typedef std::vector<Attribute> AttributeList;

  /// State attribute list type.
  typedef std::vector<AttributeList> StateAttributeList;

  // Make serializer to be friend.
  friend struct AttrFsmSerializer<AttributeFsm<Transition, Attribute> >;

  /// Default initialization.
  explicit AttributeFsm(size_t rsize = FsmImpl::kReservedStateTableSize)
    : FsmImpl(rsize)
    , states_attr_(2) {} // The automaton contains at least two states: bad and start.

  /// It may be a base class.
  virtual ~AttributeFsm() {}

  /// Get state's attribute.
  const AttributeList& GetStateAttributes(const StateId& state) const { return states_attr_[state]; }

  /**
   * \brief New state adding.
   *
   * \param is_accepted Make state acceptable.
   * \return            The added state index.
   */
  StateId AddState(bool is_accepted = false) {
    StateId state = FsmImpl::AddState(is_accepted);
    states_attr_.push_back(AttributeList());
    return state;
  }

  /// Adding attribute to the state.
  void AddAttribute(const StateId& state, const Attribute& attr) { states_attr_[state].push_back(attr); }

  StateAttributeList states_attr_; /// Attributes defined for an each state.
};

}} // namespace strutext, automata.
