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
 * \brief  Iterator for character encodings.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <boost/iterator/iterator_facade.hpp>

namespace strutext { namespace encode {

/**
 * \brief Symbol iterator implementation for character encodings.
 *
 * The class is implemented by basing on Boost iterator library. The end iterator is initialized
 * by default constructor. All end iterators are equal.
 *
 * Template parameters:
 *   ByteIterator Forward итератор of byte stream.
 *   Decoder      Decode algorithm implementation.
 */
template <typename ByteIterator, class Decoder>
class CharIterator
  : public boost::iterator_facade<CharIterator<ByteIterator, Decoder>
  , char
  , boost::forward_traversal_tag
  , uint32_t> {
private:
  // Declaration for iterator_facade.
  friend class boost::iterator_core_access;

  /**
   * \brief Increment operation implementation.
   */
  void increment() { ++it_; }

  /**
   * \brief Equality operation implementation.
   */
  bool equal(const CharIterator<ByteIterator, Decoder>& other) const {
    // All end iterators are equal.
    if (it_ == end_ and other.it_ == other.end_) {
      return true;
    }
    return it_ == other.it_;
  }

  /**
   * \brief Dereferencing operation implementation.
   */
  uint32_t dereference() const { return decoder_.Decode(*it_); }

public:
  /// Default initialization: end iterator.
  CharIterator()
    : it_()
    , end_() {}

  /**
   * \brief Initialization by byte iterator.
   *
   * \param begin Begin iterator of byte stream.
   * \param end   End iterator of byte stream.
   */
  CharIterator(ByteIterator begin, ByteIterator end)
    : it_(begin)
    , end_(end) {}

private:
  ByteIterator it_;      ///< Current byte of stream iterator.
  ByteIterator end_;     ///< End of byte stream iterator.
  Decoder      decoder_; ///< Decode algorithm implementation.
};

}} // namespace strutext, encode.
