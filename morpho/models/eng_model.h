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
 * \brief  English morphology model definition.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "model.h"

namespace strutext { namespace morpho {

/// English morpho model base abstract class definition.
struct EnglishPos : public PartOfSpeech {
  /// Type of smart pointer to the class object.
  typedef boost::shared_ptr<EnglishPos> Ptr;

  /// Possible parts of speech.
  enum PosTag {
    UNKNOWN_PS                = 0   ///< Unknown part of speech.

    , ADJECTIVE_PS            = 1
    , ADVERB_PS               = 2
    , VERB_PS                 = 3
    , NUMERAL_PS              = 4
    , NUMERAL_ORDINAL_PS      = 5
    , CONJUCTION_PS           = 6
    , INTERJECTION_PS         = 7
    , PREPOSITION_PS          = 8
    , PARTICLE_PS             = 9
    , ARTICLE_PS              = 10
    , NOUN_PS                 = 11
    , PRONOUN_PS              = 12
    , PRONOUN_NOUN_PS         = 13
    , PRONOUN_ADJECTIVE_PS    = 14
    , POSSESSIVE_PS           = 15

    , UP_BOUND_PS
  };

  /// Number.
  enum Number {
    UNKNOWN_NUMBER    = 0    ///< Unknown number.

    , SINGULAR_NUMBER = 0x01
    , PLURAL_NUMBER   = 0x02
    , UNCOUNT_NUMBER  = 0x04
    , MASS_NUMBER     = 0x08
  };

  /// Gender definition.
  enum Gender {
    UNKNOWN_GENDER     = 0 ///< Unknown gender value.

    , MASCULINE_GENDER = 1
    , FEMININE_GENDER  = 2
    , NEUTER_GENDER    = 3
  };

  /// Case definnition.
  enum Case {
    UNKNOWN_CASE      = 0 ///< Unknown case.

    , NOMINATIVE_CASE = 1
    , OBJECT_CASE     = 2
  };

  /// Time definition.
  enum Time {
    UNKNOWN_TIME          = 0 ///< Unknown time.

    , INFINITIVE_TIME     = 1
    , PRESENT_TIME        = 2
    , PAST_TIME           = 3
    , FUTURETOBE_TIME     = 4
    , PASTPARTICIPLE_TIME = 5
    , GERUND_TIME         = 6
    , IFTOBE_TIME         = 7
  };

  /// Person.
  enum Person {
    UNKNOWN_PERSON    = 0    ///< Unknown person.

    , FIRST_PERSON    = 0x01
    , SECOND_PERSON   = 0x02
    , THIRD_PERSON    = 0x04
  };

  /// Pronoun Type definition.
  enum PronounType {
    UNKNOWN_PNT          = 0 ///< Unknown pronoun type.

    , PERSONAL_PNT       = 1
    , POSSESSIVE_PNT     = 2
    , REFLEXIVE_PNT      = 3
    , DEMONSTRATIVE_PNT  = 4
  };

  /// Pronoun form definition.
  enum PronounForm {
    UNKNOWN_PNF          = 0 ///< Unknown degree.

    , PREDICATIVE_PNF = 1
    , ATTRIBUTIVE_PNF = 2
  };

  /// Adjective & Adverb degree definition.
  enum Degree {
    UNKNOWN_DEGREE       = 0 ///< Unknown degree.

    , POSITIVE_DEGREE    = 1
    , COMPARATIVE_DEGREE = 2
    , SUPERLATIVE_DEGREE = 3
  };

  /// Animation.
  enum Animation {
    UNKNOWN_ANIMATION = 0

    , ANIMATE_ANIMATION   = 0x01
    , INANIMATE_ANIMATION = 0x02
  };

  /// Voice defintion.
  enum Voice {
    UNKNOWN_VOICE = 0 ///< Unknown voice.

    , ACTIVE_VOICE  = 0x01
    , PASSIVE_VOICE = 0x02
  };

  /// Language tag.
  LanguageTag GetLangTag() const {
    return ENGLISH_LANG;
  }

  /// Class is absract one -- virtual destruction.
  virtual ~EnglishPos() {}

  /// Get part of speech tag.
  virtual PosTag GetPosTag() const = 0;

  /// Serialization implementaion.
  virtual void Serialize(uint32_t& out) const = 0;

  /// Desirialization implementation.
  virtual void Deserialize(const uint32_t& in) = 0;

  /// Write POS signature.
  static void WritePosSign(PosTag pos, uint32_t& out) {
    // Write to lower 5 bits.
    out |= static_cast<uint32_t>(pos);
  }

  /// Read POS signature.
  static PosTag ReadPosSign(const uint32_t& in) {
    return PosTag(in & 0x1f);
  }
};

namespace english {

/// Noun class definition.
struct Noun : public EnglishPos {
  /// Default initialization.
  Noun()
    : number_(UNKNOWN_NUMBER)
    , gender_(UNKNOWN_GENDER)
    , case_(UNKNOWN_CASE)
    , animation_(UNKNOWN_ANIMATION)
    , type_(UNKNOWN_PNT)
    , narrative_(false)
  {}

  /// Get part of speech tag.
  PosTag GetPosTag() const {
    return NOUN_PS;
  }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    13        1            3            2        2       2       4        5
   * --------------------------------------------------------------------------------
   * Unused | Narrative | PronounType | Animation | Case | Gender | Number | POS tag |
   * --------------------------------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(gender_) << 9;
    ob |= static_cast<uint32_t>(case_) << 11;
    ob |= static_cast<uint32_t>(animation_) << 13;
    ob |= static_cast<uint32_t>(type_) << 15;
    ob |= static_cast<uint32_t>(narrative_) << 18;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    13        1            3            2        2       2       4        5
   * --------------------------------------------------------------------------------
   * Unused | Narrative | PronounType | Animation | Case | Gender | Number | POS tag |
   * --------------------------------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_    = static_cast<Number>((ib & (0x0f << 5)) >> 5);
    gender_    = static_cast<Gender>((ib & (0x03 << 9)) >> 9);
    case_      = static_cast<Case>((ib & (0x03 << 11)) >> 11);
    animation_ = static_cast<Animation>((ib & (0x03 << 13)) >> 13);
    type_      = static_cast<PronounType>((ib & (0x07 << 15)) >> 15);
    narrative_ = static_cast<bool>((ib & 0x40000) >> 18);
  }

  Number      number_;
  Gender      gender_;
  Case        case_;
  Animation   animation_;
  PronounType type_;
  bool        narrative_;
};

/// Adjective class definition.
struct Adjective : public EnglishPos {
  Adjective()
    : degree_(UNKNOWN_DEGREE)
    , prop_(false)
  {}

  /// Get part of speech tag.
  PosTag GetPosTag() const {
    return ADJECTIVE_PS;
  }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    24      1       2        5
   * ---------------------------------
   * Unused | Prop | Degree | POS tag |
   * ---------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(degree_) << 5;
    ob |= static_cast<uint32_t>(prop_) << 7;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    24      1       2        5
   * ---------------------------------
   * Unused | Prop | Degree | POS tag |
   * ---------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    degree_ = static_cast<Degree>((ib & (0x03 << 5)) >> 5);
    prop_ = static_cast<bool>((ib & (0x01 << 7)) >> 7);
  }

  Degree degree_;
  bool   prop_;
};

/// Verb class definition.
struct Verb : public EnglishPos {
  Verb()
    : time_(UNKNOWN_TIME)
    , gender_(UNKNOWN_GENDER)
    , person_(UNKNOWN_PERSON) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return VERB_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    19       3        2       3       5
   * ------------------------------------------
   * Unused | Person | Gender | Time | POS tag |
   * ------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(time_) << 5;
    ob |= static_cast<uint32_t>(gender_) << 8;
    ob |= static_cast<uint32_t>(person_) << 10;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    19       3        2       3       5
   * ------------------------------------------
   * Unused | Person | Gender | Time | POS tag |
   * ------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    time_ = static_cast<Time>((ib & (0x07 << 5)) >> 5);
    gender_ = static_cast<Gender>((ib & (0x03 << 8)) >> 8);
    person_ = static_cast<Person>((ib & (0x07 << 10)) >> 10);
  }

  Time   time_;
  Gender gender_;
  Person person_;
};

/// Adverb class definition.
struct Adverb : public EnglishPos {
  Adverb() : degree_(UNKNOWN_DEGREE) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return ADVERB_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    25       2       5
   * ----------------------------
   * Unused | Degree | POS tag |
   * ----------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(degree_) << 5;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    25       2       5
   * ----------------------------
   * Unused | Degree | POS tag |
   * ----------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    degree_ = static_cast<Degree>((ib & (0x03 << 5)) >> 5);
  }

  Degree degree_;
};

/// Pronoun class definition.
struct Pronoun : public EnglishPos {
  Pronoun()
    : number_(UNKNOWN_NUMBER)
    , case_(UNKNOWN_CASE)
    , type_(UNKNOWN_PNT)
    , person_(UNKNOWN_PERSON) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return PRONOUN_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    15       3          3          2       4        5
   * ---------------------------------------------------------
   * Unused | Person | PronounType | Case | Number  | POS tag |
   * ---------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(case_) << 9;
    ob |= static_cast<uint32_t>(type_) << 11;
    ob |= static_cast<uint32_t>(person_) << 14;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    15       3          3          2       4        5
   * ---------------------------------------------------------
   * Unused | Person | PronounType | Case | Number  | POS tag |
   * ---------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_  = static_cast<Number>((ib & (0x0f << 5)) >> 5);
    case_  = static_cast<Case>((ib & (0x03 << 9)) >> 9);
    type_    = static_cast<PronounType>((ib & (0x07 << 11)) >> 11);
    person_ = static_cast<Person>((ib & (0x07 << 14)) >> 14);
  }

  Number      number_;
  Case        case_;
  PronounType type_;
  Person      person_;
};

/// Pronoun adjective class definition.
struct PronounAdjective : public EnglishPos {
  PronounAdjective()
    : number_(UNKNOWN_NUMBER)
    , form_(UNKNOWN_PNF)
    , type_(UNKNOWN_PNT) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return PRONOUN_ADJECTIVE_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    18         3             2          4        5
   * ------------------------------------------------------
   * Unused | PronounType | PronounForm | Number | POS tag |
   * ------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(form_) << 9;
    ob |= static_cast<uint32_t>(type_) << 11;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    18         3             2          4        5
   * ------------------------------------------------------
   * Unused | PronounType | PronounForm | Number | POS tag |
   * ------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_  = static_cast<Number>((ib & (0x0f << 5)) >> 5);
    form_  = static_cast<PronounForm>((ib & (0x03 << 9)) >> 9);
    type_    = static_cast<PronounType>((ib & (0x07 << 11)) >> 11);
  }

  Number         number_;
  PronounForm    form_;
  PronounType    type_;
};

/// Template class for poor pos.
template <EnglishPos::PosTag pos>
struct PoorPos : public EnglishPos {
  PoorPos() {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return pos; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    27       5
   * ------------------
   * Unused | POS tag |
   * ------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    27       5
   * ------------------
   * Unused | POS tag |
   * ------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {}
};

/// Preposion type.
typedef PoorPos<EnglishPos::PREPOSITION_PS> Preposition;

/// Conjuction type.
typedef PoorPos<EnglishPos::CONJUCTION_PS> Conjuction;

/// Interjection type.
typedef PoorPos<EnglishPos::INTERJECTION_PS> Interjection;

/// Particle type.
typedef PoorPos<EnglishPos::PARTICLE_PS> Particle;

/// Article type.
typedef PoorPos<EnglishPos::ARTICLE_PS> Article;

/// Nmeral Quantitative Article type.
typedef PoorPos<EnglishPos::NUMERAL_PS> Numeral;

/// Numeral Ordinal type.
typedef PoorPos<EnglishPos::NUMERAL_ORDINAL_PS> NumeralOrdinal;

/// Pronoun Predicative type.
typedef PoorPos<EnglishPos::PRONOUN_NOUN_PS> PronounNoun;

/// Possessive type.
typedef PoorPos<EnglishPos::POSSESSIVE_PS> Possessive;

/// Serialization implementation.
struct PosSerializer {
  /**
   * \brief Serialization.
   *
   * \param       pos Pointer to POS class object.
   * \param[out]  ob  Buffer to write to.
   */
  static void Serialize(EnglishPos::Ptr pos, uint32_t& ob) {
    ob = 0;
    EnglishPos::WritePosSign(pos->GetPosTag(), ob);
    pos->Serialize(ob);
  }

  /**
   * \brief Deserialization.
   * \param ob Buffer to read from.
   */
  static EnglishPos::Ptr Deserialize(const uint32_t& ob) {
    EnglishPos::PosTag pos_tag = EnglishPos::ReadPosSign(ob);
    EnglishPos::Ptr pos;
    switch (pos_tag) {
      case EnglishPos::NOUN_PS:
        pos = boost::make_shared<Noun>();
        break;
      case EnglishPos::ADJECTIVE_PS:
        pos = boost::make_shared<Adjective>();
        break;
      case EnglishPos::PRONOUN_PS:
        pos = boost::make_shared<Pronoun>();
        break;
      case EnglishPos::PRONOUN_NOUN_PS:
        pos = boost::make_shared<PronounNoun>();
        break;
      case EnglishPos::VERB_PS:
        pos = boost::make_shared<Verb>();
        break;
      case EnglishPos::PRONOUN_ADJECTIVE_PS:
        pos = boost::make_shared<PronounAdjective>();
        break;
      case EnglishPos::NUMERAL_PS:
        pos = boost::make_shared<Numeral>();
        break;
      case EnglishPos::NUMERAL_ORDINAL_PS:
        pos = boost::make_shared<NumeralOrdinal>();
        break;
      case EnglishPos::ADVERB_PS:
        pos = boost::make_shared<Adverb>();
        break;
      case EnglishPos::PREPOSITION_PS:
        pos = boost::make_shared<Preposition>();
        break;
      case EnglishPos::CONJUCTION_PS:
        pos = boost::make_shared<Conjuction>();
        break;
      case EnglishPos::INTERJECTION_PS:
        pos = boost::make_shared<Interjection>();
        break;
      case EnglishPos::PARTICLE_PS:
        pos = boost::make_shared<Particle>();
        break;
      case EnglishPos::ARTICLE_PS:
        pos = boost::make_shared<Article>();
        break;
      case EnglishPos::POSSESSIVE_PS:
        pos = boost::make_shared<Possessive>();
        break;
      default:
        assert(false and "incorrect pos value");
    }

    pos->Deserialize(ob);
    return pos;
  }
};

}}} // namespace strutext, morpho, english.
