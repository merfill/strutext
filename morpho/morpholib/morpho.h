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
 * \brief  Morphology library definition.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <memory>
#include <list>
#include <utility>
#include <iterator>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "utf8_iterator.h"
#include "utf8_generator.h"
#include "flex_transitions.h"
#include "serializer.h"
#include "trie.h"
#include "fsm_defs.h"
#include "alphabet.h"
#include "suffix_storage.h"
#include "base_storage.h"

namespace strutext { namespace morpho {

const std::string kNullSuffix = std::string(1, '\0'); ///< Empty suffix

// forward declaration of MorphoModifier class.
class MorphoModifier;

/// Base morphologist class.
struct MorphologistBase : private boost::noncopyable {
  /// Smart pointer type.
  typedef boost::shared_ptr<MorphologistBase> Ptr;

  /// Lemma type definion.
  struct Lemma {
    /// Initialization of all fields.
    Lemma(uint32_t id, uint32_t attr)
      : id_(id)
      , attr_(attr) {}

    /// Default initilization.
    Lemma()
      : id_(0)
      , attr_(0) {}

    uint32_t id_;   ///< Lemma identifier.
    uint32_t attr_; ///< Form attributes.
  };

  /// Lemma list type definition.
  typedef std::list<Lemma> LemList;

  /// Virtual destruction: the class is inheritable.
  virtual ~MorphologistBase() {}

  /**
   * \brief Implementation of morphological analysis of passed form.
   *
   * \param      text     Input text in UTF-8 encoding.
   * \param[out] lem_list List of lemmas within morphological attributes.
   */
  virtual void Analize(const std::string& text, LemList& lem_list) const = 0;

  /**
   * \brief Generate form.
   *
   * \param lem_id The lemma identifier.
   * \param attrs  The attributes of the form.
   * \return       Generated text in UTF-8 encoding.
   */
  virtual std::string Generate(uint32_t lem_id, uint32_t attrs) const = 0;

  /**
   * \brief Generate all lemma's forms.
   *
   * \param      lem_id   The lemma identifier.
   * \param[out] form_set Set of forms generated as UTF-8 texts.
   * \return              Number of generated forms.
   */
  virtual size_t GenAllForms(uint32_t lem_id, std::set<std::string>& form_set) const = 0;

  /**
   * \brief Get main form.
   *
   * \param lem_id    The lemma identifier.
   * \param main_form The main form's text (UTF-8).
   * \return          True if frm is found.
   */
  virtual bool GenMainForm(uint32_t lem_id, std::string& main_form) const = 0;

  /// Serialization implementation.
  virtual void Serialize(std::ostream& os) const = 0;

  /// Deserialization implementation.
  virtual void Deserialize(std::istream& is) = 0;
};

/**
 * \brief Morphology library implementation.
 * Template parameter is Alphabet interface implementation.
 */
template <class T>
class Morphologist : public MorphologistBase {
  /// Alphabet type definition.
  typedef Alphabet<T> AlphabetImpl;

  /// Character type definition.
  typedef typename AlphabetImpl::Code Code;

  /// Type of symbol.
  typedef typename AlphabetImpl::Symbol Symbol;

  /// Type of transition table.
  typedef strutext::automata::FlexTransitions<Code> MoveTable;

  /// Type of attribute of the state.
  typedef uint64_t Attribute;

  /// Trie type definition.
  typedef strutext::automata::Trie<MoveTable, Attribute> Trie;

  /// Trie serializer type definition.
  typedef strutext::automata::AttrFsmSerializer<Trie> TrieSerializer;

  /// Map of automata 64bit attribute for morphology.
  struct AttrMap {
    union {
      struct {
        uint32_t lem_id_;     ///< Lemma identifier.
        uint32_t line_id_;    ///< Line id.
      };
      uint64_t   auto_attr_;  ///< Whole automaton attribute.
    };
  };

  /// Friend MorphoModifier class.
  friend class strutext::morpho::MorphoModifier;

public:
  /**
   * \brief Implementation of morphological analysis of passed form.
   *
   * \param      text     Input text in UTF-8 encoding.
   * \param[out] lem_list List of lemmas within morphological attributes.
   */
  void Analize(const std::string& text, LemList& lem_list) const {
    // The first phase. Go throw the passed word text, encode symbol
    // and remember symbol codes in the string. If found word base on
    // some position, remember attribute and position for an each
    // attribute.

    // Try starts with empty bases
    typedef std::list<std::pair<Attribute, size_t> > BaseList;
    BaseList base_list;
    strutext::automata::StateId state = strutext::automata::kStartState;
    if (bases_trie_.IsAcceptable(state))  {
      const typename Trie::AttributeList& attrs = bases_trie_.GetStateAttributes(state);
      for (size_t i = 0; i < attrs.size(); ++i) {
        base_list.push_back(std::make_pair(attrs[i], 0));
      }
    }

    // Permorm the first phase.
    std::string code_str;
    typedef strutext::encode::Utf8Iterator<std::string::const_iterator> Utf8Iterator;
    for (Utf8Iterator sym_it(text.begin(), text.end()); sym_it != Utf8Iterator(); ++sym_it) {
      Code c = alphabet_.Encode(*sym_it);
      code_str += c;
      if (state != strutext::automata::kInvalidState) {
        state = bases_trie_.Go(state, c);
        if (bases_trie_.IsAcceptable(state))  {
          const typename Trie::AttributeList& attrs = bases_trie_.GetStateAttributes(state);
          for (size_t i = 0; i < attrs.size(); ++i) {
            base_list.push_back(std::make_pair(attrs[i], code_str.size()));
          }
        }
      }
    }

    // The second phase. Go throuth the found base list and find suffixes for them.
    // If suffixes have been found then add them to the lemma list.
    lem_list.clear();
    for (BaseList::iterator base_it = base_list.begin(); base_it != base_list.end(); ++base_it) {
      AttrMap attr;
      attr.auto_attr_ = base_it->first;
      SuffixStorage::AttrList att_list;
      std::string suffix = code_str.substr(base_it->second);
      // If suffix is empty (empty suffix passed), add zero symbol to it.
      if (suffix.empty()) {
        suffix.push_back('\0');
      }
      if (const SuffixStorage::AttrList* att_list = suff_store_.SearchAttrs(attr.line_id_, suffix)) {
        for (size_t i = 0; i < att_list->size(); ++i) {
          lem_list.push_back(Lemma(attr.lem_id_, (*att_list)[i]));
        }
      }
    }
  }

  /**
   * \brief Generate form.
   *
   * \param lem_id The lemma identifier.
   * \param attrs  The attributes of the form.
   * \return       Generated text in UTF-8 encoding.
   */
  std::string Generate(uint32_t lem_id, uint32_t attrs) const {
    // Firstly, find the lemma's base and line.
    std::string base_text;
    uint32_t line_id = 0;
    std::string result;
    if (base_store_.Search(lem_id, line_id, base_text)) {
      // Then search suffix for the attribute.
      if (const std::string* suffix = suff_store_.SearchSuffix(line_id, attrs)) {
        // Write base text.
        for (std::string::iterator sym_it = base_text.begin(); sym_it != base_text.end(); ++sym_it) {
          strutext::encode::GetUtf8Sequence(alphabet_.Decode(*sym_it), std::back_inserter(result));
        }
        // Write suffix.
        if (*suffix != kNullSuffix) {
          for (std::string::const_iterator sym_it = (*suffix).begin(); sym_it != (*suffix).end(); ++sym_it) {
            strutext::encode::GetUtf8Sequence(alphabet_.Decode(*sym_it), std::back_inserter(result));
          }
        }
      }
    }
    return result;
  }

  /**
   * \brief Generate all lemma's forms.
   *
   * \param      lem_id   The lemma identifier.
   * \param[out] form_set Set of forms generated as UTF-8 texts.
   * \return              Number of generated forms.
   */
  size_t GenAllForms(uint32_t lem_id, std::set<std::string>& form_set) const {
    form_set.clear();

    // Firstly, find the lemma's base and line.
    std::string base_text;
    uint32_t line_id = 0;
    if (base_store_.Search(lem_id, line_id, base_text)) {
      // Decode base text.
      std::string base_text_utf8;
      for (std::string::iterator sym_it = base_text.begin(); sym_it != base_text.end(); ++sym_it) {
        strutext::encode::GetUtf8Sequence(alphabet_.Decode(*sym_it), std::back_inserter(base_text_utf8));
      }

      // Then get suffix set.
      std::set<std::string> suf_set;
      suff_store_.GetSuffixSet(line_id, suf_set);
      for (std::set<std::string>::iterator suf_it = suf_set.begin(); suf_it != suf_set.end(); ++suf_it) {
        // Decode suffix.
        std::string suffix_utf8;
        if (*suf_it != kNullSuffix) {
          for (std::string::const_iterator sym_it = (*suf_it).begin(); sym_it != (*suf_it).end(); ++sym_it) {
            strutext::encode::GetUtf8Sequence(alphabet_.Decode(*sym_it), std::back_inserter(suffix_utf8));
          }
        }
        // Concatenate base and suffix.
        form_set.insert(base_text_utf8 + suffix_utf8);
      }
    }
    return form_set.size();
  }

  /**
   * \brief Get main form.
   *
   * \param      lem_id    The lemma identifier.
   * \param[out] main_form The main form's text (UTF-8).
   * \return               True if frm is found.
   */
  bool GenMainForm(uint32_t lem_id, std::string& main_form) const {
    // Find the form.
    return base_store_.SearchMainForm(lem_id, main_form);
  }

  /// Serialization implementation.
  void Serialize(std::ostream& os) const {
    TrieSerializer::Serialize(bases_trie_, os);
    suff_store_.Serialize(os);
    base_store_.Serialize(os);
  }

  /// Deserialization implementation.
  void Deserialize(std::istream& is) {
    TrieSerializer::Deserialize(bases_trie_, is);
    suff_store_.Deserialize(is);
    base_store_.Deserialize(is);
  }

private:
  Trie          bases_trie_; ///< Vocabulary bases trie, used in analysis.
  BaseStorage   base_store_; ///< Base texts storage.
  SuffixStorage suff_store_; ///< Suffix storage.
  AlphabetImpl  alphabet_;   ///< Alphabet implementation.
};

}} // namespace strutext, morpho.
