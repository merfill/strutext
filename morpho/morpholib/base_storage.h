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

#pragma once

#include <stdint.h>

#include <string>
#include <map>
#include <iostream>
#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace strutext { namespace morpho {

// Forward declaration of MorphoModifier class.
class MorphoModifier;

/// Simple storage for word bases.
class BaseStorage : boost::noncopyable {
  // Using boost::serialization.
  friend class boost::serialization::access;

public:
  /// Class object pointer type.
  typedef boost::shared_ptr<BaseStorage> Ptr;

private:
  /// Lemma info.
  struct LemmaInfo {
    // Using boost::serialization.
    friend class boost::serialization::access;

    /// Boost serialization implementation.
    template<class Archive>
    void serialize(Archive& ar, const unsigned version) {
      ar & base_;
      ar & main_form_;
      ar & line_id_;
    }

    std::string base_;      ///< The base text.
    std::string main_form_; ///< The main form text.
    uint32_t    line_id_;   ///< Line id for this base.
  };

  /// Type of lem id to base and line id mapping.
  typedef std::map<uint32_t, LemmaInfo> LemInfoList;

  /// Friend declaration of modifier class.
  friend class MorphoModifier;

public:
  /**
   * \brief Get suffix attributes.
   *
   * \param      lem_id  Identifier of a lemma.
   * \param[out] line_id Line id found if any.
   * \param[out] base    Base found if any.
   * \return             True if the base having the passed id is found.
   */
  bool Search(uint32_t lem_id, uint32_t& line_id, std::string& base) const {
    LemInfoList::const_iterator info_it = storage_.find(lem_id);
    if (info_it != storage_.end()) {
      line_id = info_it->second.line_id_;
      base = info_it->second.base_;
      return true;
    }
    return false;
  }

  /**
   * \brief Get main form text.
   *
   * \param      lem_id  Identifier of a lemma.
   * \param[out] line_id Line id found if any.
   * \param[out] base    Base found if any.
   * \return             True if the base having the passed id is found.
   */
  bool SearchMainForm(uint32_t lem_id, std::string& main_form) const {
    LemInfoList::const_iterator info_it = storage_.find(lem_id);
    if (info_it != storage_.end()) {
      main_form = info_it->second.main_form_;
      return true;
    }
    return false;
  }

  /// Serialization implementation.
  void Serialize(std::ostream&) const;

  /// Deserialization implementation.
  void Deserialize(std::istream&);

private:
  /// Boost serialization implementation.
  template<class Archive>
  void serialize(Archive& ar, const unsigned version) {
    ar & storage_;
  }

  LemInfoList storage_; ///< List of lemma informations.
};

}} // namespace strutext, morpho.
