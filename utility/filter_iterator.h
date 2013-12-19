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
 * \brief  Iterator through symbols filtered and transformed by some algorithms.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <vector>
#include <stdexcept>

#include <boost/iterator/iterator_facade.hpp>

namespace strutext { namespace utility {

/**
 * \brief Iterator through symbols filtered and transformed by some algorithms.
 *
 * The iterator goes through symbol (typename Symbol) stream (typename Iterator)
 * and drops symbols by using Filter algorithm, than transforms selected symbol
 * by using Transform algoritm.
 * Example:
 *   Filter: drop all nonalphas.
 *   Transform: transform alphas to lower.
 * 'Hello, World!' --> 'helloworld'
 */
template <typename Symbol, typename Iterator, typename Filter, typename Transform>
class FilterIterator : public boost::iterator_facade<
    FilterIterator<Symbol, Iterator, Filter, Transform>,
    Symbol,
    boost::forward_traversal_tag,
    Symbol> {
  // Declaration for iterator_facade.
  friend class boost::iterator_core_access;

public:
  /**
   * \brief Initialization by iterators and also Filter and Transform algorithms.
   *
   * \param begin     Begin of element stream iterator.
   * \param end       End of element stream iterator.
   * \param filter    Filter algorithm implementation.
   * \param transform Transform algoritm implementation.
   */
  FilterIterator(Iterator begin, Iterator end, Filter filter, Transform transform)
    : symbol_()
    , filter_(filter)
    , transform_(transform)
    , it_(begin)
    , end_(end)
    , is_end_(false) {
    increment();
  }

  /// Default initialization.
  FilterIterator()
    : symbol_()
    , is_end_(true) {}

private:
  void increment() {
    is_end_ = true;
    for (; it_ != end_ and is_end_; ++it_) {
      if (filter_(*it_)) {
        symbol_ = transform_(*it_);
        is_end_ = false;
      }
    }
  }

  Symbol dereference() const {
    return symbol_;
  }

  inline bool equal(const FilterIterator& other) const {
    // All end of stream iterators are equal.
    if (is_end_ and other.is_end_) {
      return true;
    }
    return it_ == other.it_;
  }

  Symbol    symbol_;
  Filter    filter_;
  Transform transform_;
  Iterator  it_;
  Iterator  end_;
  bool      is_end_;
};

}} // namespace strutext, utility.
