/** Copyright &copy; 2011-2013, Ontologs.
 * \file   rus_aot_parser.h
 * \brief  Russian parser for AOT.
 * \author Vladimir Lapshin.
 * \date   02.04.2013
 */

#pragma once

#include <vector>
#include <set>

#include <boost/algorithm/string.hpp>

#include "aot_parser.h"
#include "rus_alphabet.h"
#include "rus_model.h"

namespace factor { namespace morpho { namespace aot {

namespace m = factor::morpho;
namespace rm = factor::morpho::russian;

class RussianAotParser : public AotParser {
  typedef std::set<std::string> MainFormCodeList;

  struct RussianAttrsParser {
    RussianAttrsParser()
      : number_(rm::RussianPos::UNKNOUN_NUMBER)
      , lang_(rm::RussianPos::NORMAL_LANG)
      , gender_(rm::RussianPos::UNKNOWN_GENDER)
      , case_(rm::RussianPos::UNKNOWN_CASE)
      , time_(rm::RussianPos::UNKNOWN_TIME)
      , person_(rm::RussianPos::UNKNOWN_PERSON)
      , entity_(rm::RussianPos::UNKNOWN_ENTITY)
      , animation_(rm::RussianPos::UNKNOWN_ANIMATION)
      , voice_(rm::RussianPos::UNKNOWN_VOICE)
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
      for (unsigned i = 0; i < attrs.size(); ++i) {
        attr_set.insert(attrs[i]);
      }

      // Go through list of attributes.
      for (unsigned i = 0; i < attrs.size(); ++i) {
        // Gender.
        if (attrs[i] == "мр") {
          gender_ = rm::RussianPos::MASCULINE_GENDER;
        } else if (attrs[i] == "жр") {
          gender_ = rm::RussianPos::FEMININE_GENDER;
        } else if (attrs[i] == "ср") {
          gender_ = rm::RussianPos::NEUTER_GENDER;
        }

        // Number
        else if (attrs[i] == "ед") {
          number_ = rm::RussianPos::SINGULAR_NUMBER;
        } else if (attrs[i] == "мн") {
          number_ = rm::RussianPos::PLURAL_NUMBER;
        }

        // Case.
        else if (attrs[i] == "им") {
          case_ = rm::RussianPos::NOMINATIVE_CASE;
        } else if (attrs[i] == "рд") {
          if (attr_set.find("2") != attr_set.end()) {
            case_ = rm::RussianPos::GENITIVE2_CASE;
          } else {
            case_ = rm::RussianPos::GENITIVE_CASE;
          }
        } else if (attrs[i] == "дт") {
          case_ = rm::RussianPos::DATIVE_CASE;
        } else if (attrs[i] == "тв") {
          case_ = rm::RussianPos::INSTRUMENTAL_CASE;
        } else if (attrs[i] == "пр") {
          if (attr_set.find("2") != attr_set.end()) {
            case_ = rm::RussianPos::PREPOSITIONAL2_CASE;
          } else {
            case_ = rm::RussianPos::PREPOSITIONAL_CASE;
          }
        } else if (attrs[i] == "вн") {
          case_ = rm::RussianPos::ACCUSATIVE_CASE;
        } else if (attrs[i] == "зв") {
          case_ = rm::RussianPos::VOCATIVE_CASE;
        }

        // Lang.
        else if (attrs[i] == "разг") {
          lang_ = rm::RussianPos::INFORMAL_LANG;
        } else if (attrs[i] == "арх") {
          lang_ = rm::RussianPos::ARCHAIZM_LANG;
        } else if (attrs[i] == "проф" or attrs[i] == "жарг") {
          lang_ = rm::RussianPos::SLANG_LANG;
        }

        // Time.
        else if (attrs[i] == "буд") {
          time_ = rm::RussianPos::FUTURE_TIME;
        } else if (attrs[i] == "нст") {
          time_ = rm::RussianPos::PRESENT_TIME;
        } else if (attrs[i] == "прш") {
          time_ = rm::RussianPos::PAST_TIME;
        }

        // Person.
        else if (attrs[i] == "1л") {
          person_ = rm::RussianPos::FIRST_PERSON;
        } else if (attrs[i] == "2л") {
          person_ = rm::RussianPos::SECOND_PERSON;
        } else if (attrs[i] == "3л") {
          person_ = rm::RussianPos::THIRD_PERSON;
        }

        // Entity.
        else if (attrs[i] == "отч") {
          entity_ = rm::RussianPos::MIDDLE_NAME_ENTITY;
        } else if (attrs[i] == "имя") {
          entity_ = rm::RussianPos::FIRST_NAME_ENTITY;
        } else if (attrs[i] == "фам") {
          entity_ = rm::RussianPos::FAMILY_NAME_ENTITY;
        } else if (attrs[i] == "аббр") {
          entity_ = rm::RussianPos::ABBREVIATION_ENTITY;
        }

        // Animation.
        else if (attrs[i] == "од") {
          animation_ = rm::RussianPos::ANIMATE_ANIMATION;
        } else if (attrs[i] == "но") {
          animation_ = rm::RussianPos::INANIMATE_ANIMATION;
        }

        // Voice.
        else if (attrs[i] == "дст") {
          voice_ = rm::RussianPos::ACTIVE_VOICE;
        } else if (attrs[i] == "стр") {
          voice_ = rm::RussianPos::PASSIVE_VOICE;
        }

        // Others.
        else if (attrs[i] == "безл") {
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

    rm::RussianPos::Number    number_;
    rm::RussianPos::Lang      lang_;
    rm::RussianPos::Gender    gender_;
    rm::RussianPos::Case      case_;
    rm::RussianPos::Time      time_;
    rm::RussianPos::Person    person_;
    rm::RussianPos::Entity    entity_;
    rm::RussianPos::Animation animation_;
    rm::RussianPos::Voice     voice_;
    bool                      impersonal_;
    bool                      brevity_;
    bool                      relativity_;
    bool                      quastionarity_;
    bool                      unchanged_;
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
    if (fields.size() >= 4 ) {
      attrs.Parse(fields[3]);
    }

    // Read part of speech.
    rm::RussianPos::Ptr pos;
    if (fields[2] == "С") { // noun.
      pos = rm::RussianPos::Ptr(new rm::Noun());
      rm::Noun* pp = static_cast<rm::Noun*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->entity_     = attrs.entity_;
    } else if (fields[2] == "П") {
      pos = rm::RussianPos::Ptr(new rm::Adjective());
      rm::Adjective* pp = static_cast<rm::Adjective*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->animation_  = attrs.animation_;
      pp->brevity_    = attrs.brevity_;
    } else if (fields[2] == "МС") {
      pos = rm::RussianPos::Ptr(new rm::PronounNoun());
      rm::PronounNoun* pp = static_cast<rm::PronounNoun*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->person_     = attrs.person_;
    } else if (fields[2] == "Г") {
      pos = rm::RussianPos::Ptr(new rm::Verb());
      rm::Verb* pp = static_cast<rm::Verb*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->voice_      = attrs.voice_;
      pp->person_     = attrs.person_;
      pp->gender_     = attrs.gender_;
      pp->impersonal_ = attrs.impersonal_;
    } else if (fields[2] == "ПРИЧАСТИЕ") {
      pos = rm::RussianPos::Ptr(new rm::Participle());
      rm::Participle* pp = static_cast<rm::Participle*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->voice_      = attrs.voice_;
      pp->case_       = attrs.case_;
      pp->gender_     = attrs.gender_;
      pp->animation_  = attrs.animation_;
    } else if (fields[2] == "ДЕЕПРИЧАСТИЕ") {
      pos = rm::RussianPos::Ptr(new rm::AdverbParticiple());
      rm::AdverbParticiple* pp = static_cast<rm::AdverbParticiple*>(pos.get());
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->voice_      = attrs.voice_;
    } else if (fields[2] == "ИНФИНИТИВ") {
      pos = rm::RussianPos::Ptr(new rm::Verb());
      rm::Verb* pp = static_cast<rm::Verb*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->voice_      = attrs.voice_;
      pp->person_     = attrs.person_;
      pp->impersonal_ = attrs.impersonal_;
    } else if (fields[2] == "МС-ПРЕДК") {
      pos = rm::RussianPos::Ptr(new rm::PronounPredicative());
      rm::PronounPredicative* pp = static_cast<rm::PronounPredicative*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->case_       = attrs.case_;
    } else if (fields[2] == "МС-П") {
      pos = rm::RussianPos::Ptr(new rm::PronounAdjective());
      rm::PronounAdjective* pp = static_cast<rm::PronounAdjective*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->animation_  = attrs.animation_;
    } else if (fields[2] == "ЧИСЛ") {
      pos = rm::RussianPos::Ptr(new rm::NumeralQuantitative());
      rm::NumeralQuantitative* pp = static_cast<rm::NumeralQuantitative*>(pos.get());
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
    } else if (fields[2] == "ЧИСЛ-П") {
      pos = rm::RussianPos::Ptr(new rm::NumeralOrdinal());
      rm::NumeralOrdinal* pp = static_cast<rm::NumeralOrdinal*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->animation_  = attrs.animation_;
    } else if (fields[2] == "Н") {
      pos = rm::RussianPos::Ptr(new rm::Adverb());
      rm::Adverb* pp = static_cast<rm::Adverb*>(pos.get());
      pp->lang_           = attrs.lang_;
      pp->relativity_     = attrs.relativity_;
      pp->quastionarity_  = attrs.quastionarity_;
      pp->brevity_        = attrs.brevity_;
    } else if (fields[2] == "ПРЕДК") {
      pos = rm::RussianPos::Ptr(new rm::Predicate());
      rm::Predicate* pp = static_cast<rm::Predicate*>(pos.get());
      pp->lang_       = attrs.lang_;
      pp->time_       = attrs.time_;
      pp->unchanged_  = attrs.unchanged_;
    } else if (fields[2] == "ПРЕДЛ") {
      pos = rm::RussianPos::Ptr(new rm::Preposition());
      rm::Preposition* pp = static_cast<rm::Preposition*>(pos.get());
      pp->lang_       = attrs.lang_;
    } else if (fields[2] == "СОЮЗ") {
      pos = rm::RussianPos::Ptr(new rm::Conjuction());
      rm::Conjuction* pp = static_cast<rm::Conjuction*>(pos.get());
      pp->lang_       = attrs.lang_;
    } else if (fields[2] == "МЕЖД") {
      pos = rm::RussianPos::Ptr(new rm::Interjection());
      rm::Interjection* pp = static_cast<rm::Interjection*>(pos.get());
      pp->lang_       = attrs.lang_;
    } else if (fields[2] == "ЧАСТ") {
      pos = rm::RussianPos::Ptr(new rm::Particle());
      rm::Particle* pp = static_cast<rm::Particle*>(pos.get());
      pp->lang_       = attrs.lang_;
    } else if (fields[2] == "ВВОДН") {
      pos = rm::RussianPos::Ptr(new rm::IntroductionaryWord());
      rm::IntroductionaryWord* pp = static_cast<rm::IntroductionaryWord*>(pos.get());
      pp->lang_       = attrs.lang_;
    } else if (fields[2] == "КР_ПРИЛ") {
      pos = rm::RussianPos::Ptr(new rm::Adjective());
      rm::Adjective* pp = static_cast<rm::Adjective*>(pos.get());
      pp->number_     = attrs.number_;
      pp->lang_       = attrs.lang_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->animation_  = attrs.animation_;
      pp->brevity_    = true;
    } else if (fields[2] == "КР_ПРИЧАСТИЕ") {
      pos = rm::RussianPos::Ptr(new rm::Participle());
      rm::Participle* pp = static_cast<rm::Participle*>(pos.get());
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
      rm::PosSerializer::Serialize(pos, ob);
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

}}} // namespace factor, morpho, aot.
