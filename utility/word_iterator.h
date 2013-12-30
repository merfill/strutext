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
 * \brief  Iterator through words (alpha sequences).
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <string>

#include <vector>
#include <stdexcept>

#include <boost/iterator/iterator_facade.hpp>

#include "symbols.h"

namespace strutext { namespace utility {

/**
 * \brief Iterator through words (alpha sequences).
 */
template <typename Symbol, typename Iterator>
class WordIterator : public boost::iterator_facade<
    WordIterator<Symbol, Iterator>,
    std::basic_string<Symbol>,
    boost::forward_traversal_tag,
    const std::basic_string<Symbol>&> {
  // Declaration for iterator_facade.
  friend class boost::iterator_core_access;

  /// Symbol sequence (word) type.
  typedef std::basic_string<Symbol> Word;

public:
  /**
   * \brief Initialization by iterators.
   *
   * \param begin     Begin of element stream iterator.
   * \param end       End of element stream iterator.
   */
  WordIterator(Iterator begin, Iterator end)
    : it_(begin)
    , end_(end) {
    increment();
  }

  /// Default initialization.
  WordIterator() {}

private:
  void increment() {
    if (it_ == end_ and not word_.empty()) {
      word_.clear();
      return;
    }

    // At first, skip whitespaces.
    for (; it_ != end_; ++it_) {
      if (symbols::IsLetter(*it_)) {
        break;
      }
    }
    word_.clear();
    for (; it_ != end_; ++it_) {
      if (symbols::IsLetter(*it_)) {
        word_ += *it_;
      } else {
        break;
      }
    }
  }

  const Word& dereference() const {
    return word_;
  }

  inline bool equal(const WordIterator& other) const {
    // All end of stream iterators are equal.
    if (it_ == end_ and other.it_ == other.end_ and word_.empty() and other.word_.empty()) {
      return true;
    }
    return it_ == other.it_ and word_ == other.word_;
  }

  Word     word_;
  Iterator it_;
  Iterator end_;
};

}} // namespace strutext, utility.
