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
 * \brief  English attributes parser for AOT.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <vector>
#include <set>
#include <string>

#include <boost/algorithm/string.hpp>

#include "aot_parser.h"
#include "eng_alphabet.h"
#include "eng_model.h"

namespace strutext { namespace morpho {

class EnglishAotParser : public AotParser {
  typedef std::set<std::string> MainFormCodeList;

  struct EnglishAttrsParser {
    EnglishAttrsParser()
      : number_(EnglishPos::UNKNOWN_NUMBER)
      , gender_(EnglishPos::UNKNOWN_GENDER)
      , case_(EnglishPos::UNKNOWN_CASE)
      , time_(EnglishPos::UNKNOWN_TIME)
      , person_(EnglishPos::UNKNOWN_PERSON)
      , animation_(EnglishPos::UNKNOWN_ANIMATION)
      , degree_(EnglishPos::UNKNOWN_DEGREE)
      , form_(EnglishPos::UNKNOWN_PNF)
      , narrative_(false)
      , geo_(false)
      , org_(false)
      , name_(false)
    {}

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
        if (attrs[i] == "f") {
          gender_ = EnglishPos::MASCULINE_GENDER;
        } else if (attrs[i] == "m") {
          gender_ = EnglishPos::FEMININE_GENDER;
        } else if (attrs[i] == "sg") {
          number_ = (EnglishPos::Number)(number_ | EnglishPos::SINGULAR_NUMBER);
        } else if (attrs[i] == "pl") {
          number_ = (EnglishPos::Number)EnglishPos::PLURAL_NUMBER;
        } else if (attrs[i] == "uncount") {
          number_ = (EnglishPos::Number)EnglishPos::UNCOUNT_NUMBER;
        } else if (attrs[i] == "mass") {
          number_ = (EnglishPos::Number)EnglishPos::MASS_NUMBER;
        } else if (attrs[i] == "inf") {
          time_ = EnglishPos::INFINITIVE_TIME;
        } else if (attrs[i] == "prsa") {
          time_ = EnglishPos::PRESENT_TIME;
        } else if (attrs[i] == "pasa") {
          time_ = EnglishPos::PAST_TIME;
        } else if (attrs[i] == "pp") {
          time_ = EnglishPos::PASTPARTICIPLE_TIME;
        } else if (attrs[i] == "ing") {
          time_ = EnglishPos::GERUND_TIME;
        } else if (attrs[i] == "fut") {
          time_ = EnglishPos::FUTURETOBE_TIME;
        } else if (attrs[i] == "if") {
          time_ = EnglishPos::IFTOBE_TIME;
        } else if (attrs[i] == "nom") {
          case_ = EnglishPos::NOMINATIVE_CASE;
        } else if (attrs[i] == "obj") {
          case_ = EnglishPos::OBJECT_CASE;
        } else if (attrs[i] == "pos") {
          degree_ = EnglishPos::POSITIVE_DEGREE;
        } else if (attrs[i] == "comp") {
          degree_ = EnglishPos::COMPARATIVE_DEGREE;
        } else if (attrs[i] == "sup") {
          degree_ = EnglishPos::SUPERLATIVE_DEGREE;
        } else if (attrs[i] == "pred") {
          form_ = EnglishPos::PREDICATIVE_PNF;
        } else if (attrs[i] == "attr") {
          form_ = EnglishPos::ATTRIBUTIVE_PNF;
        } else if (attrs[i] == "pers") {
          type_ = EnglishPos::PERSONAL_PNT;
        } else if (attrs[i] == "poss") {
          type_ = EnglishPos::POSSESSIVE_PNT;
        } else if (attrs[i] == "ref") {
          type_ = EnglishPos::REFLEXIVE_PNT;
        } else if (attrs[i] == "dem") {
          type_ = EnglishPos::DEMONSTRATIVE_PNT;
        } else if (attrs[i] == "1") {
          person_ = (EnglishPos::Person)(person_ | EnglishPos::FIRST_PERSON);
        } else if (attrs[i] == "2") {
          person_ = (EnglishPos::Person)(person_ | EnglishPos::SECOND_PERSON);
        } else if (attrs[i] == "3") {
          person_ = (EnglishPos::Person)(person_ | EnglishPos::THIRD_PERSON);
        } else if (attrs[i] == "narr") {
          narrative_ = true;
        } else if (attrs[i] == "anim") {
          animation_ = EnglishPos::ANIMATE_ANIMATION;
        } else if (attrs[i] == "geo") {
          geo_ = true;
        } else if (attrs[i] == "org") {
          org_ = true;
        } else if (attrs[i] == "name") {
          name_ = true;
        }
      }
    }

    EnglishPos::Number      number_;
    EnglishPos::Gender      gender_;
    EnglishPos::PronounType type_;
    EnglishPos::Case        case_;
    EnglishPos::Time        time_;
    EnglishPos::Person      person_;
    EnglishPos::Animation   animation_;
    EnglishPos::Degree      degree_;
    EnglishPos::PronounForm form_;
    bool                    narrative_;
    bool                    geo_;
    bool                    org_;
    bool                    name_;
  };

public:
  EnglishAotParser() {
    mf_codes_.insert("ad");
    mf_codes_.insert("ba");
    mf_codes_.insert("va");
    mf_codes_.insert("ta");
    mf_codes_.insert("pa");
    mf_codes_.insert("da");
    mf_codes_.insert("ea");
    mf_codes_.insert("na");
    mf_codes_.insert("fa");
    mf_codes_.insert("oa");
    mf_codes_.insert("ga");
    mf_codes_.insert("xa");
    mf_codes_.insert("xb");
    mf_codes_.insert("xc");
    mf_codes_.insert("xd");
    mf_codes_.insert("xf");
    mf_codes_.insert("xi");
    mf_codes_.insert("xp");
    mf_codes_.insert("yc");
    mf_codes_.insert("yd");
    mf_codes_.insert("xx");
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
    EnglishAttrsParser attrs;
    if (fields.size() >= 4) {
      attrs.Parse(fields[3]);
    }

    // Read part of speech.
    EnglishPos::Ptr pos;
    if (fields[2] == "NOUN") { // noun.
      pos = EnglishPos::Ptr(new english::Noun());
      english::Noun* pp = static_cast<english::Noun*>(pos.get());
      pp->number_     = attrs.number_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->animation_  = attrs.animation_;
      pp->narrative_  = attrs.narrative_;
    } else if (fields[2] == "ADJECTIVE") {
      pos = EnglishPos::Ptr(new english::Adjective());
      english::Adjective* pp = static_cast<english::Adjective*>(pos.get());
      pp->degree_    = attrs.degree_;
    } else if (fields[2] == "ADVERB") {
      pos = EnglishPos::Ptr(new english::Adverb());
      english::Adverb* pp = static_cast<english::Adverb*>(pos.get());
      pp->degree_     = attrs.degree_;
    } else if (fields[2] == "VERB" or fields[2] == "MOD" or fields[2] == "VBE") {
      pos = EnglishPos::Ptr(new english::Verb());
      english::Verb* pp = static_cast<english::Verb*>(pos.get());
      pp->time_       = attrs.time_;
      pp->gender_     = attrs.gender_;
      pp->person_     = attrs.person_;
    } else if (fields[2] == "PN") {
      pos = EnglishPos::Ptr(new english::Pronoun());
      english::Pronoun* pp = static_cast<english::Pronoun*>(pos.get());
      pp->number_     = attrs.number_;
      pp->case_       = attrs.case_;
      pp->type_       = attrs.type_;
      pp->person_     = attrs.person_;
    } else if (fields[2] == "PN_ADJ") {
      pos = EnglishPos::Ptr(new english::PronounAdjective());
      english::PronounAdjective* pp = static_cast<english::PronounAdjective*>(pos.get());
      pp->number_     = attrs.number_;
      pp->form_       = attrs.form_;
      pp->type_       = attrs.type_;
    } else if (fields[2] == "NUMERAL") {
      pos = EnglishPos::Ptr(new english::Numeral());
    } else if (fields[2] == "PRON") {
      pos = EnglishPos::Ptr(new english::PronounNoun());
    } else if (fields[2] == "CONJ") {
      pos = EnglishPos::Ptr(new english::Conjuction());
    } else if (fields[2] == "INT") {
      pos = EnglishPos::Ptr(new english::Interjection());
    } else if (fields[2] == "PREP") {
      pos = EnglishPos::Ptr(new english::Preposition());
    } else if (fields[2] == "PART") {
      pos = EnglishPos::Ptr(new english::Particle());
    } else if (fields[2] == "ARTICLE") {
      pos = EnglishPos::Ptr(new english::Article());
    } else if (fields[2] == "ORDNUM") {
      pos = EnglishPos::Ptr(new english::NumeralOrdinal());
    } else if (fields[2] == "POSS") {
      pos = EnglishPos::Ptr(new english::Possessive());
    }

    if (pos) {
      uint32_t ob = 0;
      english::PosSerializer::Serialize(pos, ob);
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
