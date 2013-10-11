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
 * \brief  Russian morphology model text representation.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <string>
#include <sstream>

#include "rus_model.h"

namespace strutext { namespace morpho {

/// Russian POS decription.
struct RussianPosDecription {
  /// Get part of speech description.
  static std::string GetDecription(RussianPos::Ptr pos) {
    std::stringstream st;
    switch (pos->GetPosTag()) {
      case RussianPos::UNKNOWN_PS:
        st << "неизвестная часть речи";
        break;
      case RussianPos::NOUN_PS: {
        const russian::Noun* pp = static_cast<const russian::Noun*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetGenderDescription(pp->gender_)
           << ", " << GetCaseDescription(pp->case_);
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        if (pp->entity_ != RussianPos::UNKNOWN_ENTITY) {
          st << ", " << GetEntityDescription(pp->entity_);
        }
        break;
      }
      case RussianPos::ADJECTIVE_PS: {
        const russian::Adjective* pp = static_cast<const russian::Adjective*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetGenderDescription(pp->gender_)
           << ", " << GetCaseDescription(pp->case_)
           << ", " << GetAnimationDescription(pp->animation_);
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        if (pp->brevity_) {
          st << ", краткое";
        }
        break;
      }
      case RussianPos::PRONOUN_NOUN_PS: {
        const russian::PronounNoun* pp = static_cast<const russian::PronounNoun*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetGenderDescription(pp->gender_)
           << ", " << GetCaseDescription(pp->case_)
           << ", " << GetPersonDescription(pp->person_);
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        break;
      }
      case RussianPos::VERB_PS: {
        const russian::Verb* pp = static_cast<const russian::Verb*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetTimeDescription(pp->time_)
           << ", " << GetVoiceDescription(pp->voice_)
           << ", " << GetPersonDescription(pp->person_)
           << ", " << GetGenderDescription(pp->gender_)
           << ", " << (pp->impersonal_ ? "безличная форма" : "личная форма");
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        break;
      }
      case RussianPos::PARTICIPLE_PS: {
        const russian::Participle* pp = static_cast<const russian::Participle*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetTimeDescription(pp->time_)
           << ", " << GetVoiceDescription(pp->voice_)
           << ", " << GetCaseDescription(pp->case_)
           << ", " << GetGenderDescription(pp->gender_)
           << ", " << GetAnimationDescription(pp->animation_);
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        break;
      }
      case RussianPos::ADVERB_PARTICIPLE_PS: {
        const russian::AdverbParticiple* pp = static_cast<const russian::AdverbParticiple*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetTimeDescription(pp->time_)
           << ", " << GetVoiceDescription(pp->voice_);
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        break;
      }
      case RussianPos::PRONOUN_PREDICATIVE_PS: {
        const russian::PronounPredicative* pp = static_cast<const russian::PronounPredicative*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetCaseDescription(pp->case_);
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        break;
      }
      case RussianPos::PRONOUN_ADJECTIVE_PS: {
        const russian::PronounAdjective* pp = static_cast<const russian::PronounAdjective*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetGenderDescription(pp->gender_)
           << ", " << GetCaseDescription(pp->case_)
           << ", " << GetAnimationDescription(pp->animation_);
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        break;
      }
      case RussianPos::NUMERAL_QUANTITATIVE_PS: {
        const russian::NumeralQuantitative* pp = static_cast<const russian::NumeralQuantitative*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetGenderDescription(pp->gender_)
           << ", " << GetCaseDescription(pp->case_);
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        break;
      }
      case RussianPos::NUMERAL_ORDINAL_PS: {
        const russian::NumeralOrdinal* pp = static_cast<const russian::NumeralOrdinal*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetNumberDescription(pp->number_)
           << ", " << GetGenderDescription(pp->gender_)
           << ", " << GetCaseDescription(pp->case_)
           << ", " << GetAnimationDescription(pp->animation_);
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        break;
      }
      case RussianPos::ADVERB_PS: {
        const russian::Adverb* pp = static_cast<const russian::Adverb*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << (pp->relativity_ ? "относительное" : "неотносительное")
           << ", " << (pp->quastionarity_ ? "вопросительное" : "невопросительное")
           << ", " << (pp->brevity_ ? "краткое" : "некраткое");
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        break;
      }
      case RussianPos::PREDICATE_PS: {
        const russian::Predicate* pp = static_cast<const russian::Predicate*>(pos.get());
        st << GetPosDescription(pp->GetPosTag())
           << ", " << GetTimeDescription(pp->time_)
           << ", " << (pp->unchanged_ ? "неизменяемое" : "изменяемое");
        if (pp->lang_ != RussianPos::NORMAL_LANG) {
          st << ", " << GetLangDescription(pp->lang_);
        }
        break;
      }
      case RussianPos::PREPOSITION_PS:
      case RussianPos::CONJUCTION_PS:
      case RussianPos::INTERJECTION_PS:
      case RussianPos::PARTICLE_PS:
      case RussianPos::INTRODUCTORY_WORD_PS: {
        st << GetPosDescription(pos->GetPosTag());
        break;
      }

      default:
        assert(false and "incorrect part of speech value");
    }
    return st.str();
  }

  /// Get part of speech description.
  static std::string GetPosDescription(RussianPos::PosTag pos) {
    switch (pos) {
      case RussianPos::UNKNOWN_PS:              return "неизвестное значение";
      case RussianPos::NOUN_PS:                 return "существительное";
      case RussianPos::ADJECTIVE_PS:            return "прилагательное";
      case RussianPos::PRONOUN_NOUN_PS:         return "местоимение-существительное";
      case RussianPos::VERB_PS:                 return "глагол";
      case RussianPos::PARTICIPLE_PS:           return "причастие";
      case RussianPos::ADVERB_PARTICIPLE_PS:    return "деепричастие";
      case RussianPos::PRONOUN_PREDICATIVE_PS:  return "местоимение-предикатив";
      case RussianPos::PRONOUN_ADJECTIVE_PS:    return "местоименное прилагательное";
      case RussianPos::NUMERAL_QUANTITATIVE_PS: return "числительное (количественное)";
      case RussianPos::NUMERAL_ORDINAL_PS:      return "порядковое числительное";
      case RussianPos::ADVERB_PS:               return "наречие";
      case RussianPos::PREDICATE_PS:            return "предикатив";
      case RussianPos::PREPOSITION_PS:          return "предлог";
      case RussianPos::CONJUCTION_PS:           return "союз";
      case RussianPos::INTERJECTION_PS:         return "междометие";
      case RussianPos::PARTICLE_PS:             return "частица";
      case RussianPos::INTRODUCTORY_WORD_PS:    return "вводное слово";

      default:
        assert(false and "incorrect part of speech value");
    }
    return "некорректное значение";
  }

  // Get number description.
  static std::string GetNumberDescription(RussianPos::Number number) {
    switch (number) {
      case RussianPos::UNKNOUN_NUMBER: return "неизвестное число";
      case RussianPos::SINGULAR_NUMBER: return "единственное число";
      case RussianPos::PLURAL_NUMBER: return "множественное число";

      default:
        assert(false and "incorrect value");
    }
    return "некорректное значение";
  }

  // Get Lang description.
  static std::string GetLangDescription(RussianPos::Lang lang) {
    switch (lang) {
      case RussianPos::NORMAL_LANG: return "обычный язык";
      case RussianPos::SLANG_LANG: return "профессионализм";
      case RussianPos::ARCHAIZM_LANG: return "архаизм";
      case RussianPos::INFORMAL_LANG: return "неформальный язык";

      default:
        assert(false and "incorrect  value");
    }
    return "некорректное значение";
  }

  // Get Gender description.
  static std::string GetGenderDescription(RussianPos::Gender gender) {
    switch (gender) {
      case RussianPos::UNKNOWN_GENDER: return "неизвестный род";
      case RussianPos::MASCULINE_GENDER: return "мужской род";
      case RussianPos::FEMININE_GENDER: return "женский род";
      case RussianPos::NEUTER_GENDER: return "средний род";

      default:
        assert(false and "incorrect  value");
    }
    return "некорректное значение";
  }

  // Get Case description.
  static std::string GetCaseDescription(RussianPos::Case cs) {
    switch (cs) {
      case RussianPos::UNKNOWN_CASE: return "неизвестный падеж";
      case RussianPos::NOMINATIVE_CASE: return "именительный падеж";
      case RussianPos::GENITIVE_CASE: return "родительный падеж";
      case RussianPos::GENITIVE2_CASE: return "второй родительный падеж";
      case RussianPos::DATIVE_CASE: return "дательный падеж";
      case RussianPos::ACCUSATIVE_CASE: return "винительный падеж";
      case RussianPos::INSTRUMENTAL_CASE: return "творительный падеж";
      case RussianPos::PREPOSITIONAL_CASE: return "предложный падеж";
      case RussianPos::PREPOSITIONAL2_CASE: return "второй предложный падеж";
      case RussianPos::VOCATIVE_CASE: return "звательный падеж";

      default:
        assert(false and "incorrect  value");
    }
    return "некорректное значение";
  }

  // Get Time description.
  static std::string GetTimeDescription(RussianPos::Time time) {
    switch (time) {
      case RussianPos::UNKNOWN_TIME: return "неизвестное время";
      case RussianPos::PRESENT_TIME: return "настояшее время";
      case RussianPos::FUTURE_TIME: return "будущее время";
      case RussianPos::PAST_TIME: return "прошедшее время";

      default:
        assert(false and "incorrect  value");
    }
    return "некорректное значение";
  }

  // Get Person description.
  static std::string GetPersonDescription(RussianPos::Person person) {
    switch (person) {
      case RussianPos::UNKNOWN_PERSON: return "неизвестное лицо";
      case RussianPos::FIRST_PERSON: return "первое лицо";
      case RussianPos::SECOND_PERSON: return "второе лицо";
      case RussianPos::THIRD_PERSON: return "третье лицо";

      default:
        assert(false and "incorrect  value");
    }
    return "некорректное значение";
  }

  // Get Entity description.
  static std::string GetEntityDescription(RussianPos::Entity entity) {
    switch (entity) {
      case RussianPos::UNKNOWN_ENTITY: return "неизвестная характеристика";
      case RussianPos::ABBREVIATION_ENTITY: return "аббревиатуры";
      case RussianPos::FIRST_NAME_ENTITY: return "имена";
      case RussianPos::MIDDLE_NAME_ENTITY: return "отчества";
      case RussianPos::FAMILY_NAME_ENTITY: return "фамилии";

      default:
        assert(false and "incorrect  value");
    }
    return "некорректное значение";
  }

  // Get Animation description.
  static std::string GetAnimationDescription(RussianPos::Animation animation) {
    switch (animation) {
      case RussianPos::UNKNOWN_ANIMATION: return "неизвестная характеристика";
      case RussianPos::ANIMATE_ANIMATION: return "одушевленный";
      case RussianPos::INANIMATE_ANIMATION: return "неодушевленный";

      default:
        assert(false and "incorrect  value");
    }
    return "некорректное значение";
  }

  // Get Voice description.
  static std::string GetVoiceDescription(RussianPos::Voice voice) {
    switch (voice) {
      case RussianPos::UNKNOWN_VOICE: return "неизвестный залог";
      case RussianPos::ACTIVE_VOICE: return "действительный залог";
      case RussianPos::INANIMATE_ANIMATION: return "страдательный залог";

      default:
        assert(false and "incorrect  value");
    }
    return "некорректное значение";
  }
};

}} // namespace strutext, morpho.
