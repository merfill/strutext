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
 * \brief  Base class for all language models.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace strutext { namespace morpho {

/// Part of speech base class.
class PartOfSpeech : private boost::noncopyable {
public:
  /// Type of smart pointer to the class object.
  typedef boost::shared_ptr<PartOfSpeech> Ptr;

  /// Language tag definitions.
  enum LanguageTag {
    UNKNOWN_LANG = 0   ///< Unknown language.

    , RUSSIAN_LANG = 1 ///< Russian language.
    , ENGLISH_LANG = 2 ///< English language.
  };

  /// Language tag.
  virtual LanguageTag GetLangTag() const = 0;

  /// Virtual destruction for abstract class.
  virtual ~PartOfSpeech() {}
};

}} // namespace strutext, morpho.
