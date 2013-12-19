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
 * \brief  UTF-8 iterator on byte sequences.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <cstring>
#include <algorithm>

#include <boost/iterator/iterator_facade.hpp>

namespace strutext { namespace encode {

/**
 * \brief UTF-8 symbol description.
 */
struct Utf8Symbol {
  /// Unknown symbol initialization.
  Utf8Symbol()
    : len_(0)
    , utf32_(0)
  {}

  /// Copy construction.
  Utf8Symbol(const Utf8Symbol& other)
    : len_(other.len_)
    , utf32_(other.utf32_) {
    std::copy(other.chain_, other.chain_ + other.len_, chain_);
  }

  /// Compare operation.
  bool operator==(const Utf8Symbol& other) const {
    return std::memcmp(chain_, other.chain_, len_) == 0;
  }

  /// Assignement operation.
  Utf8Symbol& operator=(const Utf8Symbol& other) {
    if (this != &other) {
      len_ = other.len_;
      utf32_ = other.utf32_;
      std::copy(other.chain_, other.chain_ + other.len_, chain_);
    }
    return *this;
  }

  /// Casting to UTF-32 code.
  operator uint32_t() const {
    return utf32_;
  }

  uint8_t  chain_[6]; ///< UTF-8 byte sequence read.
  size_t   len_;      ///< The length of UTF-8 sequence.
  uint32_t utf32_;    ///< UTF-32 symbol code.
};

/**
 * \brief Forward iterator over byte UTF-8 stream.
 *
 * The class performs decoding of UTF-8 symbols to UNICODE32 codes. The class is implemented
 * by basing on Boost iterator library. The end iterator is initialized by default constructor.
 * All end iterators are equal.
 *
 * Template parameters:
 *   ByteIterator Forward итератор of byte stream.
 */
template <typename ByteIterator>
class Utf8Iterator : public  boost::iterator_facade<
    Utf8Iterator<ByteIterator>,
    Utf8Symbol,
    boost::forward_traversal_tag,
    uint32_t> {
  // Declaration for iterator_facade.
  friend class boost::iterator_core_access;

  /// Check the end.
  bool CheckEnd() const {
    return iter_ == end_;
  }

  /// Getting next byte from the stream.
  bool Next() {
    if (iter_ != end_ and ++iter_ != end_) {
      ++byte_pos_;
      return true;
    }
    return false;
  }

  /// Check the correctness of UTF-8 sequance.
  static bool IsLegalUtf8(const uint8_t* source, unsigned len) {
    uint8_t a;
    const uint8_t* srcptr = source + len;
    switch (len) {
      default:
        return false;
      // Last byte check.
      case 4: if ((a = (*--srcptr)) < 0x80 or a > 0xBF) return false;
      case 3: if ((a = (*--srcptr)) < 0x80 or a > 0xBF) return false;
      case 2: if ((a = (*--srcptr)) > 0xBF) return false;

      switch (*source) {
        // First and last byte comparing.
        case 0xE0: if (a < 0xA0) return false; break;
        case 0xED: if (a > 0x9F) return false; break;
        case 0xF0: if (a < 0x90) return false; break;
        case 0xF4: if (a > 0x8F) return false; break;
        default:   if (a < 0x80) return false;
      }

      case 1: if (*source >= 0x80 and *source < 0xC2) return false;
    }

    if (*source > 0xF4) {
      return false;
    }

    return true;
  }

  void RealIncrement() {
    // How many bytes in UTF-8 sequence?
    static const unsigned TRAILING_BYTES[256] = {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // NOLINT
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // NOLINT
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // NOLINT
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // NOLINT
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // NOLINT
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // NOLINT
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // NOLINT
      2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5  // NOLINT
    };
    unsigned extra_bytes_to_read = TRAILING_BYTES[static_cast<uint8_t>(*iter_)];

    symbol_.utf32_ = 0;
    symbol_.len_ = 0;

    // Go to loop of UTF-8 sequence reading.
    uint32_t decoded_symbol = 0;
    switch (extra_bytes_to_read) {
      case 5:
        symbol_.chain_[0] = *iter_;
        decoded_symbol += static_cast<uint8_t>(*iter_);
        decoded_symbol <<= 6;
        if (not Next()) {
          return;
        }
      case 4:
        symbol_.chain_[extra_bytes_to_read-4] = *iter_;
        decoded_symbol += static_cast<uint8_t>(*iter_);
        decoded_symbol <<= 6;
        if (not Next()) {
          return;
        }
      case 3:
        symbol_.chain_[extra_bytes_to_read-3] = *iter_;
        decoded_symbol += static_cast<uint8_t>(*iter_);
        decoded_symbol <<= 6;
        if (not Next()) {
          return;
        }
      case 2:
        symbol_.chain_[extra_bytes_to_read-2] = *iter_;
        decoded_symbol += static_cast<uint8_t>(*iter_);
        decoded_symbol <<= 6;
        if (not Next()) {
          return;
        }
      case 1:
        symbol_.chain_[extra_bytes_to_read-1] = *iter_;
        decoded_symbol += static_cast<uint8_t>(*iter_);
        decoded_symbol <<= 6;
        if (not Next()) {
          return;
        }
      case 0:
        symbol_.chain_[extra_bytes_to_read] = *iter_;
        decoded_symbol += static_cast<uint8_t>(*iter_);
    }

    // Magic numbers to process decoding.
    static const uint32_t OFFSETS_FROM_UTF8[6] = {
      0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL
    };
    decoded_symbol -= OFFSETS_FROM_UTF8[extra_bytes_to_read];

    symbol_.len_ = extra_bytes_to_read + 1;

    // Is the sequence legal?
    if (IsLegalUtf8((const uint8_t*)symbol_.chain_, symbol_.len_)) {
      // Increase symbol counter only, if correct symbol extracted.
      symbol_.utf32_ = decoded_symbol;
      ++sym_pos_;
    }
  }

  /// Next symbol getting.
  inline void increment() {
    if (not Next()) {
      return;
    }
    RealIncrement();
  }

  /// Compare operator.
  inline bool equal(const Utf8Iterator& other) const {
    // All end iterators are equal.
    if (iter_ == end_ and other.iter_ == other.end_) {
      return true;
    }
    return iter_ == other.iter_;
  }

  /// Dereference implementation.
  uint32_t dereference() const { return symbol_.utf32_; }

public:
  /// End of stream initialization.
  Utf8Iterator()
    : iter_()
    , end_()
    , sym_pos_(0)
    , byte_pos_(0) {
  }

  /**
   * \brief Initialization of byte itreators.
   *
   * begin Begin byte stream iterator.
   * end   End byte stream iterator.
   */
  Utf8Iterator(const ByteIterator& begin, const ByteIterator& end)
    : iter_(begin)
    , end_(end)
    , sym_pos_(0)
    , byte_pos_(1) {
    RealIncrement();
  }

  /// Get byte position.
  size_t GetBytePos() const {
    return byte_pos_;
  }

  /// Get symbol position.
  size_t GetSymbolPos() const {
    return sym_pos_;
  }

  /// Get UTF-8 byte sequence.
  const uint8_t* GetChain() const {
    return symbol_.chain_;
  }

  /// Get UTF-8 byte sequence length.
  unsigned GetChainLen() const {
    return symbol_.len_;
  }

private:
  Utf8Symbol   symbol_;   ///< Current UTF-8 symbol.
  ByteIterator iter_;     ///< Current byte iterator.
  ByteIterator end_;      ///< End of byte stream iterator.
  size_t       sym_pos_;  ///< Symbol position.
  size_t       byte_pos_; ///< Byte position.
};

}} // namespace strutext, encode.
