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
 * \brief  Utilities to generate UTF-8 sequence by UNICODE32 code.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <cstdint>

#include "symbols.h"

namespace strutext { namespace encode {

/**
 * \brief Generate UTF-8 sequence from UNICODE32 symbol.
 *
 * \param code UTF-32 symbol code.
 * \param oi   Output byte iterator.
 * \param      Result iterator.
 */
template <typename ByteIterator>
inline ByteIterator GetUtf8Sequence(strutext::symbols::SymbolCode code, ByteIterator oi) {
  const uint16_t kByteMask         = 0xbfu;
  const uint16_t kByteMark         = 0x80u;
  const uint8_t  kFirstByteMark[7] = { 0x00u, 0x00u, 0xC0u, 0xE0u, 0xF0u, 0xF8u, 0xFCu };

  // How many bytes one should write?
  size_t bytes_to_write = 0;
  if (code < 0x80u) { // one byte sequence.
    bytes_to_write = 1;
  } else if (code < 0x800u) { // two bytes sequence.
    bytes_to_write = 2;
  } else if (code < 0x10000) { // three bytes sequence.
    bytes_to_write = 3;
  } else if (code <= strutext::symbols::MAX_LEGAL_UTF32) { // four bytes sequence.
    bytes_to_write = 4;
  } else { // bad symbol.
    bytes_to_write = 3;
    code = 0x0000fffdu;
  }

  // Generate UTF-8 sequence.
  uint8_t sequence[4] = {0};
  uint8_t* target = sequence + bytes_to_write;
  switch (bytes_to_write) {
    case 4:
      *--target = ((code | kByteMark) & kByteMask);
      code >>= 6;
    case 3:
      *--target = ((code | kByteMark) & kByteMask);
      code >>= 6;
    case 2:
      *--target = ((code | kByteMark) & kByteMask);
      code >>= 6;
    case 1:
      *--target = (code | kFirstByteMark[bytes_to_write]);
  }

  // And write it to output iterator.
  for (size_t i = 0; i < bytes_to_write; ++i, ++oi) {
    *oi = sequence[i];
  }
  return oi;
}

/**
 * \brief Convert UTF-32 sequence to UTF-8 sequence.
 *
 * \param begin Begin iterator of UTF-32 sequence.
 * \param end   End iterator of UTF-32 sequence.
 * \param oi    Begin iterator of UTF-8 sequence.
 * \return      Result iterator of UTF-8 sequence.
 */
template <typename Utf32Iterator, typename ByteIterator>
inline ByteIterator GetUtf8Sequence(Utf32Iterator begin, Utf32Iterator end, ByteIterator oi) {
  for (auto it = begin; it != end; ++it) {
    oi = GetUtf8Sequence(*it, oi);
  }
  return oi;
}

}} // namespace strutext, encode.
