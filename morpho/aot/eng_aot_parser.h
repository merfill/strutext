/** Copyright &copy; 2011-2013, Ontologs.
 * \file   eng_aot_parser.h
 * \brief  English attributes parser for AOT.
 * \author Vladimir Lapshin.
 * \date   02.04.2013
 */

#pragma once

#include <vector>
#include <set>

#include <boost/algorithm/string.hpp>

#include "aot_parser.h"
#include "eng_alphabet.h"
#include "eng_model.h"

namespace factor { namespace morpho { namespace aot {

namespace m = factor::morpho;
namespace em = factor::morpho::english;

class EnglishAotParser : public AotParser {
  typedef std::set<std::string> MainFormCodeList;

  struct EnglishAttrsParser {
    EnglishAttrsParser()
      : number_(em::EnglishPos::UNKNOWN_NUMBER)
      , gender_(em::EnglishPos::UNKNOWN_GENDER)
      , case_(em::EnglishPos::UNKNOWN_CASE)
      , time_(em::EnglishPos::UNKNOWN_TIME)
      , person_(em::EnglishPos::UNKNOWN_PERSON)
      , animation_(em::EnglishPos::UNKNOWN_ANIMATION)
      , degree_(em::EnglishPos::UNKNOWN_DEGREE)
      , form_(em::EnglishPos::UNKNOWN_PNF)
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
      for (unsigned i = 0; i < attrs.size(); ++i) {
        attr_set.insert(attrs[i]);
      }

      // Go through list of attributes.
      for (unsigned i = 0; i < attrs.size(); ++i) {
        // Gender.
        if (attrs[i] == "f") {
          gender_ = em::EnglishPos::MASCULINE_GENDER;
        } else if (attrs[i] == "m") {
          gender_ = em::EnglishPos::FEMININE_GENDER;
        }

        // Number
        else if (attrs[i] == "sg") {
          number_ = (em::EnglishPos::Number)(number_ | em::EnglishPos::SINGULAR_NUMBER);
        } else if (attrs[i] == "pl") {
          number_ = (em::EnglishPos::Number)em::EnglishPos::PLURAL_NUMBER;
        } else if (attrs[i] == "uncount") {
          number_ = (em::EnglishPos::Number)em::EnglishPos::UNCOUNT_NUMBER;
        } else if (attrs[i] == "mass") {
          number_ = (em::EnglishPos::Number)em::EnglishPos::MASS_NUMBER;
        }

        // Time.
        else if (attrs[i] == "inf") {
          time_ = em::EnglishPos::INFINITIVE_TIME;
        } else if (attrs[i] == "prsa") {
          time_ = em::EnglishPos::PRESENT_TIME;
        } else if (attrs[i] == "pasa") {
          time_ = em::EnglishPos::PAST_TIME;
        } else if (attrs[i] == "pp") {
          time_ = em::EnglishPos::PASTPARTICIPLE_TIME;
        } else if (attrs[i] == "ing") {
          time_ = em::EnglishPos::GERUND_TIME;
        } else if (attrs[i] == "fut") {
          time_ = em::EnglishPos::FUTURETOBE_TIME;
        } else if (attrs[i] == "if") {
          time_ = em::EnglishPos::IFTOBE_TIME;
        }

        // Case.
        else if (attrs[i] == "nom") {
          case_ = em::EnglishPos::NOMINATIVE_CASE;
        } else if (attrs[i] == "obj") {
          case_ = em::EnglishPos::OBJECT_CASE;
        }

        // Pronoun degree.
        else if (attrs[i] == "pos") {
          degree_ = em::EnglishPos::POSITIVE_DEGREE;
        } else if (attrs[i] == "comp") {
          degree_ = em::EnglishPos::COMPARATIVE_DEGREE;
        } else if (attrs[i] == "sup") {
          degree_ = em::EnglishPos::SUPERLATIVE_DEGREE;
        }

        // Pronoun form.
        else if (attrs[i] == "pred") {
          form_ = em::EnglishPos::PREDICATIVE_PNF;
        } else if (attrs[i] == "attr") {
          form_ = em::EnglishPos::ATTRIBUTIVE_PNF;
        }

        // Pronoun Type.
        else if (attrs[i] == "pers") {
          type_ = em::EnglishPos::PERSONAL_PNT;
        } else if (attrs[i] == "poss") {
          type_ = em::EnglishPos::POSSESSIVE_PNT;
        } else if (attrs[i] == "ref") {
          type_ = em::EnglishPos::REFLEXIVE_PNT;
        } else if (attrs[i] == "dem") {
          type_ = em::EnglishPos::DEMONSTRATIVE_PNT;
        }

        // Person.
        else if (attrs[i] == "1") {
          person_ = (em::EnglishPos::Person)(person_ | em::EnglishPos::FIRST_PERSON);
        } else if (attrs[i] == "2") {
          person_ = (em::EnglishPos::Person)(person_ | em::EnglishPos::SECOND_PERSON);
        } else if (attrs[i] == "3") {
          person_ = (em::EnglishPos::Person)(person_ | em::EnglishPos::THIRD_PERSON);
        }

        // Narrative.
        else if (attrs[i] == "narr") {
          narrative_ = true;
        }

        // Animation.
        else if (attrs[i] == "anim") {
          animation_ = em::EnglishPos::ANIMATE_ANIMATION;
        }

        // Geo.
        else if (attrs[i] == "geo") {
          geo_ = true;
        }

        // Org.
        else if (attrs[i] == "org") {
          org_ = true;
        }

        // Name.
        else if (attrs[i] == "name") {
          name_ = true;
        }
      }
    }

    em::EnglishPos::Number      number_;
    em::EnglishPos::Gender      gender_;
    em::EnglishPos::PronounType type_;
    em::EnglishPos::Case        case_;
    em::EnglishPos::Time        time_;
    em::EnglishPos::Person      person_;
    em::EnglishPos::Animation   animation_;
    em::EnglishPos::Degree      degree_;
    em::EnglishPos::PronounForm form_;
    bool                        narrative_;
    bool                        geo_;
    bool                        org_;
    bool                        name_;
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
    if (fields.size() >= 4 ) {
      attrs.Parse(fields[3]);
    }

    // Read part of speech.
    em::EnglishPos::Ptr pos;
    if (fields[2] == "NOUN") { // noun.
      pos = em::EnglishPos::Ptr(new em::Noun());
      em::Noun* pp = static_cast<em::Noun*>(pos.get());
      pp->number_     = attrs.number_;
      pp->gender_     = attrs.gender_;
      pp->case_       = attrs.case_;
      pp->animation_  = attrs.animation_;
      pp->narrative_  = attrs.narrative_;
    } else if (fields[2] == "ADJECTIVE") {
      pos = em::EnglishPos::Ptr(new em::Adjective());
      em::Adjective* pp = static_cast<em::Adjective*>(pos.get());
      pp->degree_    = attrs.degree_;
    } else if (fields[2] == "ADVERB") {
      pos = em::EnglishPos::Ptr(new em::Adverb());
      em::Adverb* pp = static_cast<em::Adverb*>(pos.get());
      pp->degree_     = attrs.degree_;
    } else if (fields[2] == "VERB" or fields[2] == "MOD" or fields[2] == "VBE") {
      pos = em::EnglishPos::Ptr(new em::Verb());
      em::Verb* pp = static_cast<em::Verb*>(pos.get());
      pp->time_       = attrs.time_;
      pp->gender_     = attrs.gender_;
      pp->person_     = attrs.person_;
    } else if (fields[2] == "PN") {
      pos = em::EnglishPos::Ptr(new em::Pronoun());
      em::Pronoun* pp = static_cast<em::Pronoun*>(pos.get());
      pp->number_     = attrs.number_;
      pp->case_       = attrs.case_;
      pp->type_       = attrs.type_;
      pp->person_     = attrs.person_;
    } else if (fields[2] == "PN_ADJ") {
      pos = em::EnglishPos::Ptr(new em::PronounAdjective());
      em::PronounAdjective* pp = static_cast<em::PronounAdjective*>(pos.get());
      pp->number_     = attrs.number_;
      pp->form_       = attrs.form_;
      pp->type_       = attrs.type_;
    } else if (fields[2] == "NUMERAL") {
      pos = em::EnglishPos::Ptr(new em::Numeral());
    } else if (fields[2] == "PRON") {
      pos = em::EnglishPos::Ptr(new em::PronounNoun());
    } else if (fields[2] == "CONJ") {
      pos = em::EnglishPos::Ptr(new em::Conjuction());
    } else if (fields[2] == "INT") {
      pos = em::EnglishPos::Ptr(new em::Interjection());
    } else if (fields[2] == "PREP") {
      pos = em::EnglishPos::Ptr(new em::Preposition());
    } else if (fields[2] == "PART") {
      pos = em::EnglishPos::Ptr(new em::Particle());
    } else if (fields[2] == "ARTICLE") {
      pos = em::EnglishPos::Ptr(new em::Article());
    } else if (fields[2] == "ORDNUM") {
      pos = em::EnglishPos::Ptr(new em::NumeralOrdinal());
    } else if (fields[2] == "POSS") {
      pos = em::EnglishPos::Ptr(new em::Possessive());
    }

    if (pos) {
      uint32_t ob = 0;
      em::PosSerializer::Serialize(pos, ob);
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
