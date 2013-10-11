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
 * \brief  Iterator through ngrams built on element sequences.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <vector>

#include <boost/circular_buffer.hpp>

namespace strutext { namespace lex {

template <typename Element, typename Iterator>
class NgramIterator
  public boost::iterator_facade<NgramIterator<Element, Iterator>
  , std::vector<Element>
  , boost::forward_traversal_tag
  , std::vector<Element> > {
  /// Type of circular buffer.
  typedef boost::circular_buffer<Element> Buffer;

  // Declaration for iterator_facade.
  friend class boost::iterator_core_access;

  /// Ngram returned as dereference.
  typedef std::vector<Element> Ngram;

  void increment() {
    ++ngram_len_;
    if (ngram_len_ >= buffer.size()) {
      ++it_;
      buffer_.push_back(*it_);
      ngram_len_ = min_len_;
    }
  }

  Ngram dereference() const {
    Ngram ngram(ngram_len_);
    for (size_t i = 0; i < buffer_.size(); ++i) {
      ngram_len_[i] = buffer_[i];
    }
    return ngram;
  }

  inline bool equal(const TSymbolSplitter& other) const {
    // All end of stream iterators are equal.
    if (min_len_ == 0 and other.min_len_ == 0) {
      return true;
    } else if (it_ == other.it_ and  buffer_.capacity() == other.buffer_.capacity() and ngram_len_ == other.ngram_len_) {
      return true;
    }
    return false;
  }

public:
  /**
   * \brief Initialization.
   *
   * \param begin    Begin of element stream iterator.
   * \param end      End of element stream iterator.
   * \param min_len  Minimum ngram length.
   * \param max_len  Maximum ngram length.
   */
  NrgamIterator(Iterator begin, Iterator end, size_t min_len, size_t max_len)
    : buffer_(max_len)
    , min_len_(min_len)
    , ngram_len_(0)
    , it_(begin)
    , end_(end) {
    increment();
  }

  /// Default initialization.
  NrgamIterator()
    : buffer_(0)
    , min_len_(0)
    , ngram_len_(0) {}

  Buffer   buffer_;
  size_t   min_len_;
  size_t   ngram_len_;
  Iterator it_;
  Iterator end_;
};

}} // namespace strutext, lex.
