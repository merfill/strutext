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
 * \brief  Word bases storage implementation.
 * \author Vladimir Lapshin.
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>

#include "base_storage.h"

namespace strutext { namespace morpho {

/// Serialization implementation.
void BaseStorage::Serialize(std::ostream& os) const {
  boost::archive::text_oarchive oa(os);
  oa << *this;
}

/// Deserialization implementation.
void BaseStorage::Deserialize(std::istream& is) {
  boost::archive::text_iarchive ia(is);
  ia >> *this;
}

}} // namespace strutext, morpho.
