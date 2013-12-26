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

#include <stdint.h>

#include <vector>
#include <stdexcept>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/circular_buffer.hpp>

namespace strutext { namespace utility {

template <typename Element, typename Iterator>
class NgramIterator : public boost::iterator_facade<
  NgramIterator<Element, Iterator>
  , std::vector<Element>
  , boost::forward_traversal_tag
  , std::vector<Element> > {
  /// Type of buffer for ngrams.
  typedef boost::circular_buffer<Element> Buffer;

  // Declaration for iterator_facade.
  friend class boost::iterator_core_access;

public:
  /// Ngram returned as dereference.
  typedef std::vector<Element> Ngram;

  /**
   * \brief Initialization.
   *
   * \param begin    Begin of element stream iterator.
   * \param end      End of element stream iterator.
   * \param min_len  Minimum ngram length.
   * \param max_len  Maximum ngram length.
   */
  NgramIterator(Iterator begin, Iterator end, size_t min_len, size_t max_len)
    : min_len_(min_len)
    , ngram_len_(0)
    , it_(begin)
    , end_(end) {
    // Check parameters correctness.
    if (min_len > max_len) {
      throw std::invalid_argument("Minimum ngram length is more than maximum one");
    } else if (min_len == 0) {
      throw std::invalid_argument("Minimum ngram length must not be zero");
    }

    // Fill buffer.
    buffer_.set_capacity(max_len);
    for (; it_ != end_; ++it_) {
      buffer_.push_back(*it_);
      if (buffer_.size() >= buffer_.capacity()) {
          break;
      }
    }
    ngram_len_ = buffer_.size() >= min_len_ ? min_len_ : 0;
  }

  /// Default initialization.
  NgramIterator()
    : min_len_(0)
    , ngram_len_(0) {}

private:
  void increment() {
    if (it_ == end_) {
      if (ngram_len_ >= buffer_.size()) {
        if (buffer_.size() > min_len_) {
          buffer_.pop_front();
          ngram_len_ = min_len_;
        } else {
          ngram_len_ = 0;
        }
      } else if (ngram_len_ > 0) {
        ++ngram_len_;
      }
    } else if (ngram_len_ >= buffer_.size()) {
      ++it_;
      if (it_ != end_) {
        buffer_.push_back(*it_);
        ngram_len_ = min_len_;
      } else if (buffer_.size() > min_len_){
        buffer_.pop_front();
        ngram_len_ = min_len_;
      } else {
        ngram_len_ = 0;
      }
    } else if (ngram_len_ > 0) {
      ++ngram_len_;
    }
  }

  Ngram dereference() const {
    Ngram ngram(ngram_len_);
    for (size_t i = 0; i < ngram_len_; ++i) {
      ngram[i] = buffer_[i];
    }
    return ngram;
  }

  inline bool equal(const NgramIterator& other) const {
    // All end of stream iterators are equal.
    if (ngram_len_ == 0 and other.ngram_len_ == 0) {
      return true;
    } else if (it_ == other.it_ and ngram_len_ == other.ngram_len_) {
      return true;
    }
    return false;
  }

  Buffer   buffer_;
  size_t   min_len_;
  size_t   ngram_len_;
  Iterator it_;
  Iterator end_;
};

}} // namespace strutext, utility.
