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
 * \brief  Morphology library content modifier.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <stdexcept>

#include "morpho.h"

namespace strutext { namespace morpho {

/// Modifier for all morphology classes.
struct MorphoModifier {
  /**
   * \brief Add base to the vocabulary.
   *
   * \param morph     Morphologist class object.
   * \param lem_id    Lemma identifier.
   * \param line_id   Suffix line identifier.
   * \param begin     Begin iterator of base text (UNICODE).
   * \param end       End iterator of base text (UNICODE).
   * \param main_form The lemma's main form.
   */
  template <class Alphabet, class Iterator>
  static void AddBase(Morphologist<Alphabet>& morph, uint32_t lem_id, uint32_t line_id, Iterator begin, Iterator end,
                        const std::string& main_form) {
    // At first encode the passed string.
    std::string code_base;
    for (Iterator sym_it = begin; sym_it != end; ++sym_it) {
      typename Morphologist<Alphabet>::Code c = morph.alphabet_.Encode(*sym_it);
      code_base += c;
    }

    // Then add the encoded base to morpho trie.
    typename Morphologist<Alphabet>::AttrMap attr_map;
    attr_map.lem_id_ = lem_id;
    attr_map.line_id_ = line_id;
    morph.bases_trie_.AddChain(code_base.begin(), code_base.end(), attr_map.auto_attr_);

    // Add lem id to base map and line id to base storage.
    BaseStorage::LemmaInfo lem_info;
    lem_info.base_ = code_base;
    lem_info.main_form_ = main_form;
    lem_info.line_id_ = line_id;
    morph.base_store_.storage_.insert(std::make_pair(lem_id, lem_info));
  }

  /**
   * \brief Add new suffix line into the suffix storage.
   *
   * \param  morph Morphologist class object.
   * \return       Line identifier.
   */
  template <class Alphabet>
  static uint32_t AddSuffixLine(Morphologist<Alphabet>& morph) {
    morph.suff_store_.suff_storage_.push_back(SuffixStorage::SuffixLine());
    morph.suff_store_.attr_storage_.push_back(SuffixStorage::AttrLine());
    return morph.suff_store_.suff_storage_.size() - 1;
  }

  /**
   * \brief Add suffix to some line.
   *
   * \param morph   Morphologist class object.
   * \param line_id Line identifier.
   * \param attrs   Form attributes packed.
   * \param begin   The suffix text begin iterator.
   * \param end     The suffix text begin iterator.
   */
  template <class Alphabet, class Iterator>
  static void AddSuffix(Morphologist<Alphabet>& morph, uint32_t line_id, uint32_t attrs, Iterator begin, Iterator end) {
    if (morph.suff_store_.suff_storage_.size() <= line_id) {
      throw std::invalid_argument("invalid line id passed");
    }

    // At first encode the passed string.
    std::string code_suffix;
    for (Iterator sym_it = begin; sym_it != end; ++sym_it) {
      typename Morphologist<Alphabet>::Code c = morph.alphabet_.Encode(*sym_it);
      code_suffix += c;
    }

    // If code_suffix is empty (empty suffix passed), add zero symbol to code_suffix.
    if (code_suffix.empty()) {
      code_suffix.push_back('\0');
    }

    // Find suffix into the line.
    SuffixStorage::SuffixLine::iterator it = morph.suff_store_.suff_storage_[line_id].find(code_suffix);
    if (it == morph.suff_store_.suff_storage_[line_id].end()) {
      SuffixStorage::AttrList attr_list;
      attr_list.push_back(attrs);
      morph.suff_store_.suff_storage_[line_id].insert(std::make_pair(code_suffix, attr_list));
    } else {
      it->second.push_back(attrs);
    }

    // Add line id to suffix relation.
    morph.suff_store_.attr_storage_[line_id].insert(std::make_pair(attrs, code_suffix));
  }
};

}} // namespace strutext, morpho.
