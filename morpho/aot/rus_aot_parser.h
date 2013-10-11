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
 * \brief  Russian parser for AOT.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <vector>
#include <set>
#include <string>

#include <boost/algorithm/string.hpp>

#include "aot_parser.h"
#include "rus_alphabet.h"
#include "rus_model.h"

namespace strutext { namespace morpho {

class RussianAotParser : public AotParser {
  typedef std::set<std::string> MainFormCodeList;

  struct RussianAttrsParser {
    RussianAttrsParser()
      : number_(RussianPos::UNKNOUN_NUMBER)
      , lang_(RussianPos::NORMAL_LANG)
      , gender_(RussianPos::UNKNOWN_GENDER)
      , case_(RussianPos::UNKNOWN_CASE)
      , time_(RussianPos::UNKNOWN_TIME)
      , person_(RussianPos::UNKNOWN_PERSON)
      , entity_(RussianPos::UNKNOWN_ENTITY)
      , animation_(RussianPos::UNKNOWN_ANIMATION)
      , voice_(RussianPos::UNKNOWN_VOICE)
      , impersonal_(false)
      , brevity_(false)
      , relativity_(false)
      , quastionarity_(false)
      , unchanged_(false) {
    }

    void Parse(const std::string& attrs_str) {
      std::vector<std::string> attrs;
      boost::algorithm::split(attrs, attrs_str, boost::is_any_of(","));

      typedef std::set<std::string> AttrSet;
      AttrSet attr_set;
      for (size_t i = 0; i < attrs.size(); ++i) {
        attr_set.insert(attrs[i]);
      }

      // Go through list of attributes.
      for (size_t i = 0; i < attrs.size(); ++i) {
        // Gender.
        if (attrs[i] == "мр") {
          gender_ = RussianPos::MASCULINE_GENDER;
        } else if (attrs[i] == "жр") {
          gender_ = RussianPos::FEMININE_GENDER;
        } else if (attrs[i] == "ср") {
          gender_ = RussianPos::NEUTER_GENDER;
        } else if (attrs[i] == "ед") {
          number_ = RussianPos::SINGULAR_NUMBER;
        } else if (attrs[i] == "мн") {
          number_ = RussianPos::PLURAL_NUMBER;
        } else if (attrs[i] == "им") {
          case_ = RussianPos::NOMINATIVE_CASE;
        } else if (attrs[i] == "рд") {
          if (attr_set.find("2") != attr_set.end()) {
            case_ = RussianPos::GENITIVE2_CASE;
          } else {
            case_ = RussianPos::GENITIVE_CASE;
          }
        } else if (attrs[i] == "дт") {
          case_ = RussianPos::DATIVE_CASE;
        } else if (attrs[i] == "тв") {
          case_ = RussianPos::INSTRUMENTAL_CASE;
        } else if (attrs[i] == "пр") {
          if (attr_set.find("2") != attr_set.end()) {
            case_ = RussianPos::PREPOSITIONAL2_CASE;
          } else {
            case_ = RussianPos::PREPOSITIONAL_CASE;
          }
        } else if (attrs[i] == "вн") {
          case_ = RussianPos::ACCUSATIVE_CASE;
        } else if (attrs[i] == "зв") {
          case_ = RussianPos::VOCATIVE_CASE;
        } else if (attrs[i] == "разг") {
          lang_ = RussianPos::INFORMAL_LANG;
        } else if (attrs[i] == "арх") {
          lang_ = RussianPos::ARCHAIZM_LANG;
        } else if (attrs[i] == "проф" or attrs[i] == "жарг") {
          lang_ = RussianPos::SLANG_LANG;
        } else if (attrs[i] == "буд") {
          time_ = RussianPos::FUTURE_TIME;
        } else if (attrs[i] == "нст") {
          time_ = RussianPos::PRESENT_TIME;
        } else if (attrs[i] == "прш") {
          time_ = RussianPos::PAST_TIME;
        } else if (attrs[i] == "1л") {
          person_ = RussianPos::FIRST_PERSON;
        } else if (attrs[i] == "2л") {
          person_ = RussianPos::SECOND_PERSON;
        } else if (attrs[i] == "3л") {
          person_ = RussianPos::THIRD_PERSON;
        } else if (attrs[i] == "отч") {
          entity_ = RussianPos::MIDDLE_NAME_ENTITY;
        } else if (attrs[i] == "имя") {
          entity_ = RussianPos::FIRST_NAME_ENTITY;
        } else if (attrs[i] == "фам") {
          entity_ = RussianPos::FAMILY_NAME_ENTITY;
        } else if (attrs[i] == "аббр") {
          entity_ = RussianPos::ABBREVIATION_ENTITY;
        } else if (attrs[i] == "од") {
          animation_ = RussianPos::ANIMATE_ANIMATION;
        } else if (attrs[i] == "но") {
          animation_ = RussianPos::INANIMATE_ANIMATION;
        } else if (attrs[i] == "дст") {
          voice_ = RussianPos::ACTIVE_VOICE;
        } else if (attrs[i] == "стр") {
          voice_ = RussianPos::PASSIVE_VOICE;
        } else if (attrs[i] == "безл") {
          impersonal_ = true;
        } else if (attrs[i] == "кр") {
          brevity_ = true;
        } else if (attrs[i] == "относ") {
          relativity_ = true;
        } else if (attrs[i] == "вопр") {
          quastionarity_ = true;
        } else if (attrs[i] == "0") {
          unchanged_ = true;
        }
      }
    }

    RussianPos::Number    number_;
    RussianPos::Lang      lang_;
    RussianPos::Gender    gender_;
    RussianPos::Case      case_;
    RussianPos::Time      time_;
    RussianPos::Person    person_;
    RussianPos::Entity    entity_;
    RussianPos::Animation animation_;
    RussianPos::Voice     voice_;
    bool                  impersonal_;
    bool                  brevity_;
    bool                  relativity_;
    bool                  quastionarity_;
    bool                  unchanged_;
  };

public:
  RussianAotParser() {
    mf_codes_.insert("ша");
    mf_codes_.insert("шж");
    mf_codes_.insert("шт");
    mf_codes_.insert("шм");
    mf_codes_.insert("чж");
    mf_codes_.insert("аа");
    mf_codes_.insert("Юо");
    mf_codes_.insert("го");
    mf_codes_.insert("ва");
    mf_codes_.insert("во");
    mf_codes_.insert("га");
    mf_codes_.insert("Йа");
    mf_codes_.insert("Йм");
    mf_codes_.insert("еа");
    mf_codes_.insert("Яз");
    mf_codes_.insert("иж");
    mf_codes_.insert("ао");
    mf_codes_.insert("ап");
    mf_codes_.insert("ат");
    mf_codes_.insert("ау");
    mf_codes_.insert("ац");
    mf_codes_.insert("ач");
    mf_codes_.insert("аъ");
    mf_codes_.insert("бо");
    mf_codes_.insert("бН");
    mf_codes_.insert("вН");
    mf_codes_.insert("вО");
    mf_codes_.insert("до");
    mf_codes_.insert("дН");
    mf_codes_.insert("Ра");
    mf_codes_.insert("Рж");
    mf_codes_.insert("Та");
    mf_codes_.insert("Тж");
    mf_codes_.insert("йа");
    mf_codes_.insert("иа");
    mf_codes_.insert("нр");
    mf_codes_.insert("ка");
    mf_codes_.insert("кн");
    mf_codes_.insert("ко");
    mf_codes_.insert("Эт");
    mf_codes_.insert("Эу");
    mf_codes_.insert("ла");
    mf_codes_.insert("ма");
    mf_codes_.insert("па");
    mf_codes_.insert("са");
    mf_codes_.insert("ча");
    mf_codes_.insert("ыа");
    mf_codes_.insert("ыш");
    mf_codes_.insert("ыь");
    mf_codes_.insert("эа");
    mf_codes_.insert("Ца");
    mf_codes_.insert("эж");
    mf_codes_.insert("юа");
    mf_codes_.insert("яа");
    mf_codes_.insert("яб");
    mf_codes_.insert("яв");
    mf_codes_.insert("яг");
    mf_codes_.insert("яе");
    mf_codes_.insert("яд");
    mf_codes_.insert("яё");
    mf_codes_.insert("яж");
    mf_codes_.insert("яз");
    mf_codes_.insert("яй");
  }

  /// Parser attributes definition line.
  uint32_t ParseAttrLine(const std::string& line, std::string& id) const {
    // Split line onto fields.
    std::vector<std::string> fields;
    boost::algorithm::split(fields, line, boost::is_any_of(" "));
    // At least three fields must be present.
    if (fields.size() < 3) {
      throw std::invalid_argument(("incorrect number of fields in tab file: \"" + line + "\"").c_str());
    }

    // Remember attrs identifier.
    id = fields[0];

    // Extract morphological attributes.
    RussianAttrsParser attrs;
    if (fields.size() >= 4) {
      attrs.Parse(fields[3]);
    }

    // Read part of speech.
    RussianPos::Ptr pos;
    if (fields[2] == "С") { // noun.
      pos = RussianPos::Ptr(new russian::Noun());
      russian::Noun* pp = static_cast<russian::Noun*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->entity_     = attrs.entity_;
    } else if (fields[2] == "П") {
      pos = RussianPos::Ptr(new russian::Adjective());
      russian::Adjective* pp = static_cast<russian::Adjective*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->animation_  = attrs.animation_;
      pp->brevity_    = attrs.brevity_;
    } else if (fields[2] == "МС") {
      pos = RussianPos::Ptr(new russian::PronounNoun());
      russian::PronounNoun* pp = static_cast<russian::PronounNoun*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->person_     = attrs.person_;
    } else if (fields[2] == "Г") {
      pos = RussianPos::Ptr(new russian::Verb());
      russian::Verb* pp = static_cast<russian::Verb*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->voice_      = attrs.voice_;
      pp->person_     = attrs.person_;
      pp->gender_     = attrs.gender_;
      pp->impersonal_ = attrs.impersonal_;
    } else if (fields[2] == "ПРИЧАСТИЕ") {
      pos = RussianPos::Ptr(new russian::Participle());
      russian::Participle* pp = static_cast<russian::Participle*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->voice_      = attrs.voice_;
      pp->case_       = attrs.case_;
      pp->gender_     = attrs.gender_;
      pp->animation_  = attrs.animation_;
    } else if (fields[2] == "ДЕЕПРИЧАСТИЕ") {
      pos = RussianPos::Ptr(new russian::AdverbParticiple());
      russian::AdverbParticiple* pp = static_cast<russian::AdverbParticiple*>(pos.get());
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->voice_      = attrs.voice_;
    } else if (fields[2] == "ИНФИНИТИВ") {
      pos = RussianPos::Ptr(new russian::Verb());
      russian::Verb* pp = static_cast<russian::Verb*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->voice_      = attrs.voice_;
      pp->person_     = attrs.person_;
      pp->impersonal_ = attrs.impersonal_;
    } else if (fields[2] == "МС-ПРЕДК") {
      pos = RussianPos::Ptr(new russian::PronounPredicative());
      russian::PronounPredicative* pp = static_cast<russian::PronounPredicative*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->case_       = attrs.case_;
    } else if (fields[2] == "МС-П") {
      pos = RussianPos::Ptr(new russian::PronounAdjective());
      russian::PronounAdjective* pp = static_cast<russian::PronounAdjective*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->animation_  = attrs.animation_;
    } else if (fields[2] == "ЧИСЛ") {
      pos = RussianPos::Ptr(new russian::NumeralQuantitative());
      russian::NumeralQuantitative* pp = static_cast<russian::NumeralQuantitative*>(pos.get());
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
    } else if (fields[2] == "ЧИСЛ-П") {
      pos = RussianPos::Ptr(new russian::NumeralOrdinal());
      russian::NumeralOrdinal* pp = static_cast<russian::NumeralOrdinal*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->animation_  = attrs.animation_;
    } else if (fields[2] == "Н") {
      pos = RussianPos::Ptr(new russian::Adverb());
      russian::Adverb* pp = static_cast<russian::Adverb*>(pos.get());
      pp->lang_           = attrs.lang_;
      pp->relativity_     = attrs.relativity_;
      pp->quastionarity_  = attrs.quastionarity_;
      pp->brevity_        = attrs.brevity_;
    } else if (fields[2] == "ПРЕДК") {
      pos = RussianPos::Ptr(new russian::Predicate());
      russian::Predicate* pp = static_cast<russian::Predicate*>(pos.get());
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->unchanged_  = attrs.unchanged_;
    } else if (fields[2] == "ПРЕДЛ") {
      pos = RussianPos::Ptr(new russian::Preposition());
      russian::Preposition* pp = static_cast<russian::Preposition*>(pos.get());
      pp->lang_       = attrs.lang_;
    } else if (fields[2] == "СОЮЗ") {
      pos = RussianPos::Ptr(new russian::Conjuction());
      russian::Conjuction* pp = static_cast<russian::Conjuction*>(pos.get());
      pp->lang_       = attrs.lang_;
    } else if (fields[2] == "МЕЖД") {
      pos = RussianPos::Ptr(new russian::Interjection());
      russian::Interjection* pp = static_cast<russian::Interjection*>(pos.get());
      pp->lang_       = attrs.lang_;
    } else if (fields[2] == "ЧАСТ") {
      pos = RussianPos::Ptr(new russian::Particle());
      russian::Particle* pp = static_cast<russian::Particle*>(pos.get());
      pp->lang_       = attrs.lang_;
    } else if (fields[2] == "ВВОДН") {
      pos = RussianPos::Ptr(new russian::IntroductionaryWord());
      russian::IntroductionaryWord* pp = static_cast<russian::IntroductionaryWord*>(pos.get());
      pp->lang_       = attrs.lang_;
    } else if (fields[2] == "КР_ПРИЛ") {
      pos = RussianPos::Ptr(new russian::Adjective());
      russian::Adjective* pp = static_cast<russian::Adjective*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->animation_  = attrs.animation_;
      pp->brevity_    = true;
    } else if (fields[2] == "КР_ПРИЧАСТИЕ") {
      pos = RussianPos::Ptr(new russian::Participle());
      russian::Participle* pp = static_cast<russian::Participle*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->voice_      = attrs.voice_;
      pp->case_       = attrs.case_;
      pp->gender_     = attrs.gender_;
      pp->animation_  = attrs.animation_;
    }

    if (pos.get()) {
      uint32_t ob = 0;
      russian::PosSerializer::Serialize(pos, ob);
      return ob;
    }
    return 0;
  }

  /// Check is passed string main form code.
  bool IsMainFormCode(const std::string& code) const {
    if (mf_codes_.find(code) != mf_codes_.end()) {
      return true;
    }
    return false;
  }

private:
  MainFormCodeList mf_codes_;
};

}} // namespace strutext, morpho.
