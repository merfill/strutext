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
 * \brief  Word suffices storage implementation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <stdexcept>
#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>

namespace strutext { namespace morpho {

// Forward declaration of MorphoModifier class.
class MorphoModifier;

/**
 * \brief Suffix storage implementation.
 *
 * Suffix storage contains a list of "suffix lines", identified by index started from 0.
 * An each line is a map, where key is a string, representing a suffix, and value is
 * a list of 32bit attributes. To find attribute list one must pass line id and suffix
 * value to the seach procedure.
 */
class SuffixStorage : boost::noncopyable {
  // Using boost::serialization.
  friend class boost::serialization::access;

public:
  /// Class object pointer type.
  typedef boost::shared_ptr<SuffixStorage> Ptr;

  /// Type of attribute array.
  typedef std::vector<uint32_t> AttrList;

private:
  /// Type of suffix list element.
  typedef std::map<std::string, AttrList> SuffixLine;

  /// Type of attributes' suffix list.
  typedef std::map<uint32_t, std::string> AttrLine;

  /// Type of suffix line list.
  typedef std::vector<SuffixLine> SuffAttrStorage;

  /// Type of attribures' suffix line list.
  typedef std::vector<AttrLine> AttrSuffStorage;

  /// Friend declaration of modifier class.
  friend class MorphoModifier;

public:
  /**
   * \brief Get suffix attributes.
   *
   * \param  line_id Identifier of line where suffix must be find.
   * \param  suffix  Suffix value.
   * \return         Found attributes pointer or NULL.
   */
  const AttrList* SearchAttrs(size_t line_id, const std::string& suffix) const {
    if (line_id < suff_storage_.size()) {
      SuffixLine::const_iterator it = suff_storage_[line_id].find(suffix);
      if (it != suff_storage_[line_id].end()) {
        return &(it->second);
      }
    } else {
      throw std::invalid_argument("incorrect line id passed");
    }
    return NULL;
  }

  /**
   * \brief Get attribute's suffix.
   *
   * \param  line_id Identifier of line where suffix must be find.
   * \param  suffix  Suffix value.
   * \return         Found attributes pointer or NULL.
   */
  const std::string* SearchSuffix(size_t line_id, uint32_t attr) const {
    if (line_id < attr_storage_.size()) {
      AttrLine::const_iterator it = attr_storage_[line_id].find(attr);
      if (it != attr_storage_[line_id].end()) {
        return &(it->second);
      }
    } else {
      throw std::invalid_argument("incorrect line id passed");
    }
    return NULL;
  }

  /**
   * \brief Get all line suffixes.
   *
   * \param      line_id Identifier of line where suffix must be find.
   * \param[out] suf_set Suffixes found.
   */
  void GetSuffixSet(size_t line_id, std::set<std::string>& suf_set) const {
    suf_set.clear();
    if (line_id < attr_storage_.size()) {
      for (AttrLine::const_iterator it = attr_storage_[line_id].begin(); it != attr_storage_[line_id].end(); ++it) {
        suf_set.insert(it->second);
      }
    } else {
      throw std::invalid_argument("incorrect line id passed");
    }
  }

  /// Serialization implementation.
  void Serialize(std::ostream&) const;

  /// Deserialization implementation.
  void Deserialize(std::istream& is);

private:
  /// Boost serialization implementation.
  template<class Archive>
  void serialize(Archive& ar, const unsigned version) {
    ar & suff_storage_;
    ar & attr_storage_;
  }

  SuffAttrStorage suff_storage_; ///< Storage of suffixes and attributes.
  AttrSuffStorage attr_storage_; ///< Storage of attributes and suffixes.
};

}} // namespace strutext, morpho.
