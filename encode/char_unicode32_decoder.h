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
 * \brief  Byte to UNICODE32 decode interface.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

namespace strutext { namespace encode {

/**
 * \brief Base byte to UNICODE32 decoder class definition.
 *
 * The class is built basing on static polimorphism model implemented by
 * using CRTP [http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern].
 */
template <typename Impl> class CharUnicode32Decoder {
public:
  /**
   * \brief Decode byte to UNICODE32.
   *
   * \param  byte Byte to decode.
   * \return      UNNICODE32 symbol decoded.
   */
  uint32_t Decode(uint8_t byte) const {
    return static_cast<const Impl*>(this)->DecodeImpl(byte);
  }
};

/// CP1251 decoder.
class Cp1251Decoder: public CharUnicode32Decoder<Cp1251Decoder> {
public:
  uint32_t DecodeImpl(uint8_t byte) const { return table_[byte]; }
  static const uint32_t table_[256];
};

/// CP1252 decoder.
class Cp1252Decoder: public CharUnicode32Decoder<Cp1252Decoder> {
public:
  uint32_t DecodeImpl(uint8_t byte) const { return table_[byte]; }
  static const uint32_t table_[256];
};

/// CP1253 decoder.
class Cp1253Decoder: public CharUnicode32Decoder<Cp1253Decoder> {
public:
  uint32_t DecodeImpl(uint8_t byte) const { return table_[byte]; }
  static const uint32_t table_[256];
};

/// Cp866 decoder.
class Cp866Decoder: public CharUnicode32Decoder<Cp866Decoder> {
public:
  uint32_t DecodeImpl(uint8_t byte) const { return table_[byte]; }
  static const uint32_t table_[256];
};

/// Iso8859-1 decoder.
class Iso88591Decoder: public CharUnicode32Decoder<Iso88591Decoder> {
public:
  uint32_t DecodeImpl(uint8_t byte) const { return table_[byte]; }
  static const uint32_t table_[256];
};

/// Koi8r decoder.
class Koi8rDecoder: public CharUnicode32Decoder<Koi8rDecoder> {
public:
  uint32_t DecodeImpl(uint8_t byte) const { return table_[byte]; }
  static const uint32_t table_[256];
};

/// Koi8ru decoder.
class Koi8ruDecoder: public CharUnicode32Decoder<Koi8ruDecoder> {
public:
  uint32_t DecodeImpl(uint8_t byte) const { return table_[byte]; }
  static const uint32_t table_[256];
};

/// Koi8u decoder.
class Koi8uDecoder: public CharUnicode32Decoder<Koi8uDecoder> {
public:
  uint32_t DecodeImpl(uint8_t byte) const { return table_[byte]; }
  static const uint32_t table_[256];
};

/// MacCyrillic decoder.
class MacCyrillicDecoder: public CharUnicode32Decoder<MacCyrillicDecoder> {
public:
  uint32_t DecodeImpl(uint8_t byte) const { return table_[byte]; }
  static const uint32_t table_[256];
};

/// MacUkraine decoder.
class MacUkraineDecoder: public CharUnicode32Decoder<MacUkraineDecoder> {
public:
  uint32_t DecodeImpl(uint8_t byte) const { return table_[byte]; }
  static const uint32_t table_[256];
};

}} // namespace strutext, encode.
