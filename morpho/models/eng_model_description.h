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
 * \brief  English morphology model text representation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <string>
#include <sstream>

#include "eng_model.h"

namespace strutext { namespace morpho {

/// English description.
struct EnglishPosDescription {
  /// Get part of speech description.
  static std::string GetDescription(EnglishPos* pos) {
    std::stringstream st;
    switch (pos->GetPosTag()) {
      case EnglishPos::UNKNOWN_PS:
        st << "unknown part of speech";
        break;
      case EnglishPos::NOUN_PS: {
        const english::Noun* pp = static_cast<const english::Noun*>(pos);
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetGenderDescription(pp->gender_)
           << ", " << GetCaseDescription(pp->case_)
           << ", " << GetAnimationDescription(pp->animation_)
           << ", " << GetPronounTypeDescription(pp->type_);
        if (pp->narrative_) {
          st << ", narrative";
        }
        break;
      }
      case EnglishPos::ADJECTIVE_PS: {
        const english::Adjective* pp = static_cast<const english::Adjective*>(pos);
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetDegreeDescription(pp->degree_);
        if (pp->prop_) {
          st << ", propper name";
        }
        break;
      }
      case EnglishPos::VERB_PS: {
        const english::Verb* pp = static_cast<const english::Verb*>(pos);
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetTimeDescription(pp->time_)
           << ", " << GetGenderDescription(pp->gender_)
           << ", " << GetPersonDescription(pp->person_);
        break;
      }
      case EnglishPos::ADVERB_PS: {
        const english::Adverb* pp = static_cast<const english::Adverb*>(pos);
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetDegreeDescription(pp->degree_);
        break;
      }
      case EnglishPos::PRONOUN_PS: {
        const english::Pronoun* pp = static_cast<const english::Pronoun*>(pos);
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetCaseDescription(pp->case_)
           << ", " << GetPronounTypeDescription(pp->type_)
           << ", " << GetPersonDescription(pp->person_);
        break;
      }
      case EnglishPos::PRONOUN_ADJECTIVE_PS: {
        const english::PronounAdjective* pp = static_cast<const english::PronounAdjective*>(pos);
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetPronounFormDescription(pp->form_)
           << ", " << GetPronounTypeDescription(pp->type_);
        break;
      }
      case EnglishPos::NUMERAL_PS:
      case EnglishPos::NUMERAL_ORDINAL_PS:
      case EnglishPos::CONJUCTION_PS:
      case EnglishPos::INTERJECTION_PS:
      case EnglishPos::PREPOSITION_PS:
      case EnglishPos::PARTICLE_PS:
      case EnglishPos::ARTICLE_PS:
      case EnglishPos::PRONOUN_NOUN_PS:
      case EnglishPos::POSSESSIVE_PS:
        st << GetPosDescription(pos->GetPosTag());
        break;

      default:
        assert(false and "incorrect part of speech value");
    }
    return st.str();
  }

  /// Get part of speech description.
  static std::string GetPosDescription(EnglishPos::PosTag pos) {
    switch (pos) {
      case EnglishPos::ADJECTIVE_PS:         return "adjective";
      case EnglishPos::ADVERB_PS:            return "abverb";
      case EnglishPos::VERB_PS:              return "verb";
      case EnglishPos::NUMERAL_PS:           return "numeral";
      case EnglishPos::NUMERAL_ORDINAL_PS:   return "numeral-ordinal";
      case EnglishPos::CONJUCTION_PS:        return "conjuction";
      case EnglishPos::INTERJECTION_PS:      return "interjection";
      case EnglishPos::PREPOSITION_PS:       return "preposition";
      case EnglishPos::PARTICLE_PS:          return "particle";
      case EnglishPos::ARTICLE_PS:           return "article";
      case EnglishPos::NOUN_PS:              return "noun";
      case EnglishPos::PRONOUN_PS:           return "pronoun";
      case EnglishPos::PRONOUN_NOUN_PS:      return "pronoun-noun";
      case EnglishPos::PRONOUN_ADJECTIVE_PS: return "pronoun-adjective";
      case EnglishPos::POSSESSIVE_PS:        return "possesive";

      default:
        assert(false and "incorrect part of speech value");
    }
    return "";
  }

  // Get number description.
  static std::string GetNumberDescription(EnglishPos::Number number) {
    if (number == EnglishPos::UNKNOWN_NUMBER) {
      return "unknown number";
    } else if (number & EnglishPos::SINGULAR_NUMBER) {
      return "singular number";
    } else if (number & EnglishPos::PLURAL_NUMBER) {
      return "plural number";
    } else if (number & EnglishPos::UNCOUNT_NUMBER) {
      return "uncount number";
    } else if (number & EnglishPos::MASS_NUMBER) {
      return "mass number";
    } else {
        assert(false and "incorrect value");
    }
    return "";
  }

  // Get Gender description.
  static std::string GetGenderDescription(EnglishPos::Gender gender) {
    switch (gender) {
      case EnglishPos::UNKNOWN_GENDER:   return "unknown gender";
      case EnglishPos::MASCULINE_GENDER: return "masculine gender";
      case EnglishPos::FEMININE_GENDER:  return "feminine gender";
      case EnglishPos::NEUTER_GENDER:    return "neuter gender";

      default:
        assert(false and "incorrect  value");
    }
    return "";
  }

  // Get Case description.
  static std::string GetCaseDescription(EnglishPos::Case cs) {
    switch (cs) {
      case EnglishPos::UNKNOWN_CASE:    return "unknown case";
      case EnglishPos::NOMINATIVE_CASE: return "nominative case";
      case EnglishPos::OBJECT_CASE:     return "object case";

      default:
        assert(false and "incorrect  value");
    }
    return "некорректное значение";
  }

  // Get Time description.
  static std::string GetTimeDescription(EnglishPos::Time time) {
    switch (time) {
      case EnglishPos::UNKNOWN_TIME:        return "unknown time";
      case EnglishPos::INFINITIVE_TIME:     return "infinitive time";
      case EnglishPos::PRESENT_TIME:        return "present time";
      case EnglishPos::PAST_TIME:           return "past time";
      case EnglishPos::FUTURETOBE_TIME:     return "future to be time";
      case EnglishPos::PASTPARTICIPLE_TIME: return "past participle time";
      case EnglishPos::GERUND_TIME:         return "gerund time";
      case EnglishPos::IFTOBE_TIME:         return "if to be time";

      default:
        assert(false and "incorrect  value");
    }
    return "";
  }

  // Get Person description.
  static std::string GetPersonDescription(EnglishPos::Person person) {
    switch (person) {
      case EnglishPos::UNKNOWN_PERSON: return "unknown person";
      case EnglishPos::FIRST_PERSON:   return "first person";
      case EnglishPos::SECOND_PERSON:  return "second person";
      case EnglishPos::THIRD_PERSON:   return "third person";

      default:
        assert(false and "incorrect  value");
    }
    return "некорректное значение";
  }

  // Get Pronoun type description.
  static std::string GetPronounTypeDescription(EnglishPos::PronounType type) {
    switch (type) {
      case EnglishPos::UNKNOWN_PNT:       return "unknown pronoun type";
      case EnglishPos::PERSONAL_PNT:      return "personal pronoun type";
      case EnglishPos::POSSESSIVE_PNT:    return "possessive pronoun type";
      case EnglishPos::REFLEXIVE_PNT:     return "reflexive pronoun type";
      case EnglishPos::DEMONSTRATIVE_PNT: return "demostrative pronoun type";

      default:
        assert(false and "incorrect  value");
    }
    return "";
  }

  // Get Pronoun Form description.
  static std::string GetPronounFormDescription(EnglishPos::PronounForm form) {
    switch (form) {
      case EnglishPos::UNKNOWN_PNF:     return "unknown pronoun form";
      case EnglishPos::PREDICATIVE_PNF: return "predicative pronoun form";
      case EnglishPos::ATTRIBUTIVE_PNF: return "attributive pronoun form";

      default:
        assert(false and "incorrect  value");
    }
    return "";
  }

  // Get Degree description.
  static std::string GetDegreeDescription(EnglishPos::Degree degree) {
    switch (degree) {
      case EnglishPos::UNKNOWN_DEGREE:     return "unknown degree";
      case EnglishPos::POSITIVE_DEGREE:    return "positive degree";
      case EnglishPos::COMPARATIVE_DEGREE: return "comparative degree";
      case EnglishPos::SUPERLATIVE_DEGREE: return "superlative degree";

      default:
        assert(false and "incorrect  value");
    }
    return "";
  }

  // Get Animation description.
  static std::string GetAnimationDescription(EnglishPos::Animation animation) {
    if (animation == EnglishPos::UNKNOWN_ANIMATION) {
      return "unknown animation";
    } else if (animation & EnglishPos::ANIMATE_ANIMATION) {
      return "animate animation";
    } else if  (animation & EnglishPos::INANIMATE_ANIMATION) {
      return "inanimate animation";
    } else {
      assert(false and "incorrect  value");
    }
    return "";
  }
};

}} // namespace strutext, morpho.
