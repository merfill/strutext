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
 * \brief  Search iterators basing on AC-automaton.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <string>

#include <boost/iterator/iterator_facade.hpp>

#include "aho_corasick.h"

namespace strutext { namespace automata {

/**
 * \brief Symbol iterator, which implements search using Aho-Corasick trie.
 */
template <class AcTrie, typename SymbolIterator>
class AcSymbolIterator : public boost::iterator_facade<
  AcSymbolIterator<AcTrie, SymbolIterator>,
  StateId,
  boost::forward_traversal_tag,
  const typename AcTrie::AttributeList&> {
private:
  /// Declaration for iterator_facade.
  friend class boost::iterator_core_access;

  /// Symbol type.
  typedef typename AcTrie::Transitions::CharType SymbolType;

  /// Aho-Corasick trie searcher type.
  typedef AcProcessor<AcTrie> AcProcessorImpl;

  /// Type of chain identifier.
  typedef typename AcTrie::ChainId ChainId;

  /// Type of list of attributes.
  typedef typename AcTrie::AttributeList AttributeList;

public:
  /**
   * \brief Default initialization.
   */
  AcSymbolIterator()
    : state_(kInvalidState)
    , ac_trie_(AcTrie()) {}

  /**
   * \brief Initialization by symbol stream's iterators.
   *
   * \param begin   Iterator of the stream's begin.
   * \param end     Iterator of the stream's end.
   * \param ac_trie The trie reference.
   */
  AcSymbolIterator(const SymbolIterator& begin, const SymbolIterator& end, const AcTrie& ac_trie)
    : state_(kStartState)
    , it_(begin)
    , end_(end)
    , ac_trie_(ac_trie) {
    increment();
  }

private:
  /**
   * \brief Move by the next symbol.
   */
  void increment() {
    if (state_ != kInvalidState and it_ != end_) {
      AcProcessorImpl ac_processor(ac_trie_);
      state_ = ac_processor.Move(state_, *it_);
      ++it_;
    }
  }

  /**
   * \brief Iterators' equal operator.
   * \param other The iterator to compare.
   */
  bool equal(const AcSymbolIterator& other) const {
    // All end iterators are equal.
    if (it_ == end_ and other.it_ == other.end_) {
      return true;
    }
    return it_ == other.it_;
  }

  /**
   * \brief The attributes' set related to the current state.
   */
  const AttributeList& dereference() const {
    return ac_trie_.GetStateAttributes(state_);
  }

private:
  StateId        state_;   ///< The current automaton state.
  SymbolIterator it_;      ///< The current symbol iterator.
  SymbolIterator end_;     ///< End of stream iterator.
  const AcTrie&  ac_trie_; ///< Aho-Corasick trie object reference.
};

/**
 * \brief Iterator by chains, found by Aho-Corasick automaton.
 */
template <class AcTrie, typename SymbolIterator>
class AcChainIterator: public boost::iterator_facade<
  AcChainIterator<AcTrie, SymbolIterator>,
  typename AcTrie::ChainId,
  boost::forward_traversal_tag,
  typename AcTrie::ChainId> {
private:
  // Declaration for iterator_facade.
  friend class boost::iterator_core_access;

  /// Aho-Corasick trie processor type.
  typedef AcProcessor<AcTrie> AcProcessorImpl;

  /// Symbol type.
  typedef typename AcTrie::CharType SymbolType;

  /// Type of chain identifier.
  typedef typename AcTrie::ChainId ChainId;

  /// Type of list of attributes.
  typedef typename AcTrie::AttributeList AttributeList;

public:
  /**
   * \brief Default initialization.
   */
  AcChainIterator()
    : state_(kInvalidState)
    , index_(0)
    , pos_(0)
    , ac_trie_(AcTrie()) {}

  /**
   * \brief initialization by input stream.
   *
   * \param begin   Iterator of the stream's begin.
   * \param end     Iterator of the stream's end.
   * \param ac_trie The trie.
   */
  AcChainIterator(const SymbolIterator& begin, const SymbolIterator& end, const AcTrie& ac_trie)
    : state_(kStartState)
    , index_(0)
    , pos_(0)
    , it_(begin)
    , end_(end)
    , ac_trie_(ac_trie) {
    increment();
  }

  /**
   * \brief Return the current position.
   */
  size_t GetPos() const {
    return pos_;
  }

private:
  /**
   * \brief Next symbol move.
   */
  void increment() {
    // At first, iterate through chains, found on the current position or go
    // to the next position.
    const AttributeList& attrs = ac_trie_.GetStateAttributes(state_);
    if (++index_ >= attrs.size()) {
      bool next = true;
      AcProcessorImpl ac_processor(ac_trie_);
      for (; next and it_ != end_; ++it_, ++pos_) {
        state_ = ac_processor.Move(state_, *it_);
        const AttributeList& attrs = ac_trie_.GetStateAttributes(state_);
        if (attrs.size()) {
          index_ = 0;
          next = false;
        }
      }
    }
  }

  /**
   * \brief Compare iterators.
   * \param  other The iterator object to compare.
   */
  bool equal(const AcChainIterator& other) const {
    if (it_ == end_ and other.it_ == other.end_) {
      const AttributeList& attrs = ac_trie_.GetStateAttributes(state_);
      if (index_ >= attrs.size()) {
        return true;
      }
    }
    return it_ == other.it_ and index_ == other.index_;
  }

  /**
   * \brief Return current chain.
   */
  ChainId dereference() const {
    const AttributeList& attrs = ac_trie_.GetStateAttributes(state_);
    if (index_ < attrs.size()) {
      return attrs[index_];
    }
    return 0;
  }

private:
  StateId        state_;   ///< The current state.
  size_t         index_;   ///< The current index in the chain ids' set.
  size_t         pos_;     ///< The current symbol position.
  SymbolIterator it_;      ///< The current iterator.
  SymbolIterator end_;     ///< End of stream iterator.
  const AcTrie&  ac_trie_; ///< Aho-Corasick trie reference.
};

}} // namespace strutext, automata.
