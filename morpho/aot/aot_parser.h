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
 * \brief  AOT parser interface definition.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <string>
#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace strutext { namespace morpho {

class AotParser : boost::noncopyable {
public:
  /// Smart pointer type definition.
  typedef boost::shared_ptr<AotParser> Ptr;

  /// Check is passed string main form code.
  virtual uint32_t ParseAttrLine(const std::string& line, std::string& id) const = 0;

  /// Check code correctness.
  virtual bool IsMainFormCode(const std::string& code) const = 0;
};

}} // namespace strutext, morpho.
