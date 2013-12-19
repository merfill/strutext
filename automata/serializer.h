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
 * \brief  Finite State Machine serializer.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <iostream>
#include <stdexcept>
#include <vector>

#include "flat_transitions.h"
#include "flex_transitions.h"
#include "fsm.h"
#include "attr_fsm.h"
#include "aho_corasick.h"

namespace strutext { namespace automata {

/**
 * \brief Forward declaration of move serializer.
 *
 * Concrete type serializers will be defined below as partial specializations.
 */
template <class TransImpl> class TransSerializer;

/// Flat transition table serializer.
template <typename Char>
class TransSerializer<FlexTransitions<Char> > {
public:
  /// Move table type.
  typedef FlexTransitions<Char> Transitions;

  /**
   * \brief Serialization implementation.
   *
   * \param moves Move table.
   * \param os    Stream to write to.
   */
  static void Serialize(const Transitions& moves, std::ostream& os) {
    // Firstly, write number of moves.
    uint32_t num_of_moves = moves.trans_table_.size();
    os.write((const char*)&num_of_moves, sizeof num_of_moves);
    if (os.bad()) {
      throw std::runtime_error("Cannot write number of moves to stream");
    }

    // Go through the moves.
    for (typename Transitions::TransTable::const_iterator move_it = moves.trans_table_.begin(); move_it != moves.trans_table_.end(); ++move_it) {
      // Write symbol.
      os.write((const char*)&(move_it->first), sizeof(typename Transitions::CharType));
      if (os.bad()) {
        throw std::runtime_error("Cannot write symbol to stream");
      }

      // Write state.
      os.write((const char*)&(move_it->second), sizeof(StateId));
      if (os.bad()) {
        throw std::runtime_error("Cannot write state to stream");
      }
    }
  }

  /**
   * \brief Deserialization implementation.
   *
   * \param moves Move table.
   * \param is    Stream to read from.
   */
  static void Deserialize(Transitions& moves, std::istream& is) {
    // Read the number of moves.
    uint32_t num_of_moves = 0;
    is.read(reinterpret_cast<char*>(&num_of_moves), sizeof num_of_moves);

    // Calculate buffer size to read move table.
    size_t move_len = sizeof(typename Transitions::CharType) + sizeof(automata::StateId);
    size_t moves_size = num_of_moves * move_len;

    // Prepare the buffer.
    std::vector<char> buffer(moves_size);
    is.read(&buffer[0], moves_size);

    // Read the data.
    typename Transitions::TransTable::iterator lpos = moves.trans_table_.end();
    for (unsigned i = 0; i < num_of_moves; ++i) {
      lpos = moves.trans_table_.insert(lpos, std::make_pair(
          *(typename Transitions::CharType*)&buffer[i * move_len],
          *reinterpret_cast<automata::StateId*>(&buffer[i * move_len + sizeof(typename Transitions::CharType)])));
    }
  }
};

/// Flat transition table serializer.
template <typename Char, size_t Size>
  class TransSerializer<FlatTransitions<Char, Size> > {
public:
  /// Move table type.
  typedef FlatTransitions<Char, Size> Transitions;

  /**
   * \brief Serialization implementation.
   *
   * \param moves Move table.
   * \param os    Stream to write to.
   */
  static void Serialize(const Transitions& moves, std::ostream& os) {
    // Calculate the number of moves.
    uint32_t num_of_moves = 0;
    for (size_t i = 0; i < Size; ++i) {
      if (kInvalidState != moves.trans_table_[i]) {
        ++num_of_moves;
      }
    }

    // Write number of moves to the stream.
    os.write((const char*)&num_of_moves, sizeof num_of_moves);
    if (os.bad()) {
      throw std::runtime_error("Cannot write number of moves to stream");
    }

    // Write the moves.
    for (size_t i = 0; i < Size; ++i) {
      // Write the move if any.
      if (moves.trans_table_[i] == automata::kInvalidState) {
        continue;
      }

      os.write((const char*)&i, sizeof(typename Transitions::CharType));
      if (os.bad()) {
        throw std::runtime_error("Cannot write symbol to stream");
      }

       // Write the state.
       os.write((const char*)&moves.trans_table_[i], sizeof(automata::StateId));
       if (os.bad()) {
          throw std::runtime_error("Cannot write state to stream");
       }
     }
    }

   /**
    * \brief Deserialization implementation.
    *
    * \param moves Move table.
    * \param is    Stream to read from.
    */
  static void Deserialize(Transitions& moves, std::istream& is) {
    // Read number of moves.
    uint32_t num_of_moves = 0;
    is.read(reinterpret_cast<char*>(&num_of_moves), sizeof num_of_moves);

    assert(num_of_moves <= Size and "invalid number of moves read");

    // Calculate the size of move table.
    size_t move_len = sizeof(typename Transitions::CharType) + sizeof(automata::StateId);
    size_t moves_size = num_of_moves * move_len;

    // Prepare the buffer.
    std::vector<char> buffer(moves_size);
    is.read(&buffer[0], moves_size);

    // Fill the transition table.
    for (unsigned i = 0; i < num_of_moves; ++i) {
      typename Transitions::CharType sym = *(typename Transitions::CharType*)&buffer[i * move_len];
      automata::StateId state =
        *reinterpret_cast<automata::StateId*>(&buffer[i * move_len + sizeof(typename Transitions::CharType)]);
      moves.trans_table_[sym] = state;
    }
  }
};

/// Finite State Machine serialization.
template <class TransImpl>
struct FsmSerializer {
  /// Automaton type.
  typedef FiniteStateMachine<TransImpl> Automaton;

  /// Serializer type.
  typedef TransSerializer<TransImpl> TransSerializerImpl;

  /**
   * \brief Serialization implementation.
   *
   * \param fsm FSM to serialize.
   * \param os  Stream to write to.
   */
  static void Serialize(const Automaton& fsm, std::ostream& os) {
    // Write number of states.
    uint32_t num_of_states = fsm.states_.size() - 1;
    os.write((const char*)&num_of_states, sizeof num_of_states);

    // Write states.
    for (unsigned i = 1; i <= num_of_states; ++i) {
      // Write state accept sign.
      os.write((const char*)&(fsm.states_[i].is_accepted_), sizeof(uint8_t));

      // Serialize move table.
      TransSerializerImpl::Serialize(fsm.states_[i].trans_, os);
    }
  }

  /**
   * \brief Deserialization implementation.
   *
   * \param[out] fsm FSM to deserialize..
   * \param      is  Stream to read from.
   */
  static void Deserialize(Automaton& fsm, std::istream& is) {
    // Read number of states.
    uint32_t num_of_states = 0;
    is.read(reinterpret_cast<char*>(&num_of_states), sizeof num_of_states);

    // Prepare the array of states (including invalid state with index 0).
    fsm.states_.resize(num_of_states + 1);

    // Read states.
    for (unsigned i = 1; i <= num_of_states; ++i) {
      // Read accept sign.
      uint8_t is_accepted = 0;
      is.read(reinterpret_cast<char*>(&is_accepted), sizeof is_accepted);
      fsm.states_[i].is_accepted_ = is_accepted;

      // Read move table.
      TransSerializerImpl::Deserialize(fsm.states_[i].trans_, is);
    }
  }
};

/// FSM with attributes serialization.
template <class T>
struct AttrFsmSerializer {
  /// Automaton type.
  typedef T Automaton;

  /// Serializer type.
  typedef FsmSerializer<typename Automaton::Transition> FsmSerializerImpl;

  /**
   * \brief Serialization implementation.
   *
   * \param automaton Automaton to serialize.
   * \param os        Stream to write to.
   */
  static void Serialize(const Automaton& automaton, std::ostream& os) {
    // Firstly, serialize base class data.
    FsmSerializerImpl::Serialize(automaton, os);

    // Write the number of states.
    uint32_t num_of_states = automaton.states_attr_.size() - 1;
    os.write(reinterpret_cast<char*>(&num_of_states), sizeof num_of_states);

    // Write states.
    for (unsigned i = 1; i <= num_of_states; ++i) {
      // Write number of state's attributes.
      uint32_t num_of_attrs = automaton.states_attr_[i].size();
      os.write(reinterpret_cast<char*>(&num_of_attrs), sizeof num_of_attrs);

      // Write attributes.
      for (unsigned j = 0; j < num_of_attrs; ++j) {
        typename Automaton::Attribute attr = automaton.states_attr_[i][j];
        os.write(reinterpret_cast<char*>(&attr), sizeof attr);
      }
    }
  }

  /**
   * \brief Deserialization implementation.
   *
   * \param[out] automaton Automaton to deserialize.
   * \param      is        Stream to read from.
   */
  static void Deserialize(Automaton& automaton, std::istream& is) {
    // Firstly, deserialize base class data.
    FsmSerializerImpl::Deserialize(automaton, is);

    // Read the number of states.
    uint32_t num_of_states = 0;
    is.read(reinterpret_cast<char*>(&num_of_states), sizeof num_of_states);
    automaton.states_attr_.resize(num_of_states + 1);

    // Prepare buffer to read attributes.
    std::vector<typename Automaton::Attribute> buffer;

    // Read attributes.
    for (unsigned i = 1; i <= num_of_states; ++i) {
      // Read number of attributes in the state.
      uint32_t num_of_attrs = 0;
      is.read(reinterpret_cast<char*>(&num_of_attrs), sizeof num_of_attrs);

      // Read attributes of the state.
      buffer.resize(num_of_attrs);
      automaton.states_attr_[i].resize(num_of_attrs);
      is.read(reinterpret_cast<char*>(&buffer[0]), sizeof(typename Automaton::Attribute) * buffer.size());
      for (unsigned j = 0; j < num_of_attrs; ++j) {
        automaton.states_attr_[i][j] = buffer[j];
      }
    }
  }
};

/// Serialization of AC-automaton.
template <class AcTrie>
class AcSerializer {
public:
  /// Attribute serializer type.
  typedef AttrFsmSerializer<AcTrie> AttrFsmSerializerImpl;

  /**
   * \brief Serialization implementation.
   *
   * \param trie Automaton to serialize.
   * \param os   stream to write to.
   */
  static void Serialize(const AcTrie& trie, std::ostream& os) {
    // Firstly, serialize trie's data.
    AttrFsmSerializerImpl::Serialize(trie, os);

    // Write the number of states.
    uint32_t num_of_states = trie.fail_moves_.size();
    os.write(reinterpret_cast<char*>(&num_of_states), sizeof num_of_states);

    // Write fail transitions.
    if (num_of_states) {
      os.write(reinterpret_cast<const char*>(&trie.fail_moves_[0]), num_of_states * sizeof(StateId));
    }
  }

  /**
   * \brief Deserialization implementation.
   *
   * \param[out] trie Trie to serialize.
   * \param      is   Stream to write to.
   */
  static void Deserialize(AcTrie& trie, std::istream& is) {
    // Deserialize base class data.
    AttrFsmSerializerImpl::Deserialize(trie, is);

    // Read the number of states.
    uint32_t num_of_states = 0;
    is.read(reinterpret_cast<char*>(&num_of_states), sizeof num_of_states);

    // Read fail transitions.
    trie.fail_moves_.clear();
    if (num_of_states) {
      trie.fail_moves_.resize(num_of_states);
      is.read(reinterpret_cast<char*>(&trie.fail_moves_[0]), num_of_states * sizeof(StateId));
    }
  }
};

}} // namespace strutext, automata.
