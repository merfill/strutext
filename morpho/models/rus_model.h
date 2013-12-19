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
 * \brief  Russian morphology model definition.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "model.h"

namespace strutext { namespace morpho {

/// Russian morpho model base abstract class definition.
struct RussianPos : public PartOfSpeech {
  /// Type of smart pointer to the class object.
  typedef boost::shared_ptr<RussianPos> Ptr;

  /// Possible parts of speech.
  enum PosTag {
    UNKNOWN_PS                = 0 ///< Unknown part of speech.

    , NOUN_PS                 = 1   ///< существительное
    , ADJECTIVE_PS            = 2   ///< прилагательное
    , PRONOUN_NOUN_PS         = 3   ///< местоимение-существительное
    , VERB_PS                 = 4   ///< глагол в личной форме
    , PARTICIPLE_PS           = 5   ///< причастие
    , ADVERB_PARTICIPLE_PS    = 6   ///< деепричастие
    , PRONOUN_PREDICATIVE_PS  = 7   ///< местоимение-предикатив
    , PRONOUN_ADJECTIVE_PS    = 8   ///< местоименное прилагательное
    , NUMERAL_QUANTITATIVE_PS = 9   ///< числительное (количественное)
    , NUMERAL_ORDINAL_PS      = 10  ///< порядковое числительное
    , ADVERB_PS               = 11  ///< наречие
    , PREDICATE_PS            = 12  ///< предикатив
    , PREPOSITION_PS          = 13  ///< предлог
    , CONJUCTION_PS           = 14  ///< союз
    , INTERJECTION_PS         = 15  ///< междометие
    , PARTICLE_PS             = 16  ///< частица
    , INTRODUCTORY_WORD_PS    = 17  ///< вводное слово

    , UP_BOUND_PS
  };

  /// Number.
  enum Number {
    UNKNOUN_NUMBER    = 0    ///< Unknown number.

    , SINGULAR_NUMBER = 0x01 ///< Единственное.
    , PLURAL_NUMBER   = 0x02 ///< Множественное.
  };

  /// Language.
  enum Lang {
    NORMAL_LANG = 0 // Normal language.

    , SLANG_LANG = 1
    , ARCHAIZM_LANG = 2
    , INFORMAL_LANG = 3
  };

  /// Gender definitions.
  enum Gender {
    UNKNOWN_GENDER     = 0    ///< Unknown gender value.

    , MASCULINE_GENDER = 0x01 ///< мужской
    , FEMININE_GENDER  = 0x02 ///< женский
    , NEUTER_GENDER    = 0x04 ///< средний
  };

  /// Case definition.
  enum Case {
    UNKNOWN_CASE          = 0 ///< Unknown case.

    , NOMINATIVE_CASE     = 1 ///< именительный
    , GENITIVE_CASE       = 2 ///< родительный
    , GENITIVE2_CASE      = 3 ///< второй родительный
    , DATIVE_CASE         = 4 ///< дательный
    , ACCUSATIVE_CASE     = 5 ///< винительный
    , INSTRUMENTAL_CASE   = 6 ///< творительный
    , PREPOSITIONAL_CASE  = 7 ///< предложный
    , PREPOSITIONAL2_CASE = 8 ///< второй предложный
    , VOCATIVE_CASE       = 9 ///< звательный
  };

  /// Time.
  enum Time {
    UNKNOWN_TIME    = 0    ///< Unknown time.

    , PRESENT_TIME  = 0x01 ///< настоящее
    , FUTURE_TIME   = 0x02 ///< будущее
    , PAST_TIME     = 0x04 ///< прошедшее
  };

  /// Person.
  enum Person {
    UNKNOWN_PERSON    = 0    ///< Unknown person.

    , FIRST_PERSON    = 0x01 ///< первое
    , SECOND_PERSON   = 0x02 ///< второе
    , THIRD_PERSON    = 0x04 ///< третье
  };

  /// Entity kind.
  enum Entity {
    UNKNOWN_ENTITY = 0 ///< Unknown entity, for ordinal words.

    , ABBREVIATION_ENTITY = 1 ///< аббревиатуры.
    , FIRST_NAME_ENTITY   = 2 ///< имена.
    , MIDDLE_NAME_ENTITY  = 3 ///< отчества.
    , FAMILY_NAME_ENTITY  = 4 ///< фамилии.
  };

  /// Animation.
  enum Animation {
    UNKNOWN_ANIMATION = 0

    , ANIMATE_ANIMATION   = 0x01 ///< одушевленный.
    , INANIMATE_ANIMATION = 0x02 ///< неодушевленный.
  };

  /// Voice defintion.
  enum Voice {
    UNKNOWN_VOICE = 0 ///< Unknown voice.

    , ACTIVE_VOICE  = 0x01 ///< действительный залог.
    , PASSIVE_VOICE = 0x02 ///< страдательный залог.
  };

  /// Language tag.
  LanguageTag GetLangTag() const { return RUSSIAN_LANG; }

  /// Class is absract one -- virtual destruction.
  virtual ~RussianPos() {}

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

namespace russian {

/// Noun class definition.
struct Noun : public RussianPos {
  Noun()
    : number_(UNKNOUN_NUMBER)
    , lang_(NORMAL_LANG)
    , gender_(UNKNOWN_GENDER)
    , case_(UNKNOWN_CASE)
    , entity_(UNKNOWN_ENTITY) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return NOUN_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    13       3        4      3       2        2        5
   * ----------------------------------------------------------- 
   * Unused | Entity  | Case | Gender | Lang | Number | POS tag |
   * -----------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(lang_) << 7;
    ob |= static_cast<uint32_t>(gender_) << 9;
    ob |= static_cast<uint32_t>(case_) << 12;
    ob |= static_cast<uint32_t>(entity_) << 16;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    13       3        4      3       2        2        5
   * ----------------------------------------------------------- 
   * Unused | Entity  | Case | Gender | Lang | Number | POS tag |
   * -----------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_ = static_cast<Number>((ib & 0x0060) >> 5);
    lang_ = static_cast<Lang>((ib & 0x0180) >> 7);
    gender_ = static_cast<Gender>((ib & 0x0e00) >> 9);
    case_ = static_cast<Case>((ib & 0xf000) >> 12);
    entity_ = static_cast<Entity>((ib & 0x070000) >> 16);
  }

  Number number_;
  Lang   lang_;
  Gender gender_;
  Case   case_;
  Entity entity_;
};

/// Adjective class definition.
struct Adjective : public RussianPos {
  Adjective()
    : number_(UNKNOUN_NUMBER)
    , lang_(NORMAL_LANG)
    , gender_(UNKNOWN_GENDER)
    , case_(UNKNOWN_CASE)
    , animation_(UNKNOWN_ANIMATION)
    , brevity_(false) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return ADJECTIVE_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    13       1          2         4       3       2       2        5
   * -------------------------------------------------------------------------
   * Unused | Brevity | Animation  | Case | Gender | Lang | Number | POS tag |
   * -------------------------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(lang_) << 7;
    ob |= static_cast<uint32_t>(gender_) << 9;
    ob |= static_cast<uint32_t>(case_) << 12;
    ob |= static_cast<uint32_t>(animation_) << 16;
    ob |= static_cast<uint32_t>(brevity_) << 18;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    13       1          2         4       3       2       2        5
   * -------------------------------------------------------------------------
   * Unused | Brevity | Animation  | Case | Gender | Lang | Number | POS tag |
   * -------------------------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_ = static_cast<Number>((ib & 0x0060) >> 5);
    lang_ = static_cast<Lang>((ib & 0x0180) >> 7);
    gender_ = static_cast<Gender>((ib & 0x0e00) >> 9);
    case_ = static_cast<Case>((ib & 0xf000) >> 12);
    animation_ = static_cast<Animation>((ib & 0x030000) >> 16);
    brevity_ = static_cast<bool>((ib & 0x040000) >> 18);
  }

  Number    number_;
  Lang      lang_;
  Gender    gender_;
  Case      case_;
  Animation animation_;
  bool      brevity_;
};

/// Pronoun-noun class definition.
struct PronounNoun : public RussianPos {
  PronounNoun()
    : number_(UNKNOUN_NUMBER)
    , lang_(NORMAL_LANG)
    , gender_(UNKNOWN_GENDER)
    , case_(UNKNOWN_CASE)
    , person_(UNKNOWN_PERSON) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return PRONOUN_NOUN_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    13      3       4       3       2       2        5
   * -----------------------------------------------------------
   * Unused | Person | Case | Gender | Lang | Number | POS tag |
   * -----------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(lang_) << 7;
    ob |= static_cast<uint32_t>(gender_) << 9;
    ob |= static_cast<uint32_t>(case_) << 12;
    ob |= static_cast<uint32_t>(person_) << 16;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    13      3       4       3       2       2        5
   * -----------------------------------------------------------
   * Unused | Person | Case | Gender | Lang | Number | POS tag |
   * -----------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_ = static_cast<Number>((ib & 0x0060) >> 5);
    lang_ = static_cast<Lang>((ib & 0x0180) >> 7);
    gender_ = static_cast<Gender>((ib & 0x0e00) >> 9);
    case_ = static_cast<Case>((ib & 0xf000) >> 12);
    person_ = static_cast<Person>((ib & 0x070000) >> 16);
  }

  Number number_;
  Lang   lang_;
  Gender gender_;
  Case   case_;
  Person person_;
};

/// Verb class definition.
struct Verb : public RussianPos {
  Verb()
    : number_(UNKNOUN_NUMBER)
    , lang_(NORMAL_LANG)
    , time_(UNKNOWN_TIME)
    , voice_(UNKNOWN_VOICE)
    , person_(UNKNOWN_PERSON)
    , gender_(UNKNOWN_GENDER)
    , impersonal_(false) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return VERB_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    11        1           3       3        2       3      2      2        5
   * -------------------------------------------------------------------------------
   * Unused | Impersonal | Gender | Person | Voice | Time | Lang | Number | POS tag |
   * -------------------------------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(lang_) << 7;
    ob |= static_cast<uint32_t>(time_) << 9;
    ob |= static_cast<uint32_t>(voice_) << 12;
    ob |= static_cast<uint32_t>(person_) << 14;
    ob |= static_cast<uint32_t>(gender_) << 17;
    ob |= static_cast<uint32_t>(impersonal_) << 20;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    11        1           3       3        2       3      2      2        5
   * -------------------------------------------------------------------------------
   * Unused | Impersonal | Gender | Person | Voice | Time | Lang | Number | POS tag |
   * -------------------------------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_ = static_cast<Number>((ib & 0x0060) >> 5);
    lang_ = static_cast<Lang>((ib & 0x0180) >> 7);
    time_ = static_cast<Time>((ib & 0x0e00) >> 9);
    voice_ = static_cast<Voice>((ib & 0x3000) >> 12);
    person_ = static_cast<Person>((ib & 0x01c000) >> 14);
    gender_ = static_cast<Gender>((ib & 0x0e0000) >> 17);
    impersonal_ = static_cast<bool>((ib & 0x100000) >> 20);
  }

  Number number_;
  Lang   lang_;
  Time   time_;
  Voice  voice_;
  Person person_;
  Gender gender_;
  bool   impersonal_;
};

/// Participle class definition.
struct Participle : public RussianPos {
  Participle()
    : number_(UNKNOUN_NUMBER)
    , lang_(NORMAL_LANG)
    , time_(UNKNOWN_TIME)
    , voice_(UNKNOWN_VOICE)
    , case_(UNKNOWN_CASE)
    , gender_(UNKNOWN_GENDER)
    , animation_(UNKNOWN_ANIMATION) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return PARTICIPLE_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    9         2         3       4      2       3      2       2         5
   * -----------------------------------------------------------------------------
   * Unused | Animation | Gender | Case | Voice | Time | Lang | Number | POS tag |
   * -----------------------------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(lang_) << 7;
    ob |= static_cast<uint32_t>(time_) << 9;
    ob |= static_cast<uint32_t>(voice_) << 12;
    ob |= static_cast<uint32_t>(case_) << 14;
    ob |= static_cast<uint32_t>(gender_) << 18;
    ob |= static_cast<uint32_t>(animation_) << 21;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    9         2         3       4      2       3      2       2         5
   * -----------------------------------------------------------------------------
   * Unused | Animation | Gender | Case | Voice | Time | Lang | Number | POS tag |
   * -----------------------------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_ = static_cast<Number>((ib & 0x0060) >> 5);
    lang_ = static_cast<Lang>((ib & 0x0180) >> 7);
    time_ = static_cast<Time>((ib & 0x0e00) >> 9);
    voice_ = static_cast<Voice>((ib & 0x3000) >> 12);
    case_ = static_cast<Case>((ib & 0x03c000) >> 14);
    gender_ = static_cast<Gender>((ib & 0x1c0000) >> 18);
    animation_ = static_cast<Animation>((ib & 0x600000) >> 21);
  }

  Number    number_;
  Lang      lang_;
  Time      time_;
  Voice     voice_;
  Case      case_;
  Gender    gender_;
  Animation animation_;
};

/// Adverb participle class definition.
struct AdverbParticiple : public RussianPos {
  AdverbParticiple()
    : lang_(NORMAL_LANG)
    , time_(UNKNOWN_TIME)
    , voice_(UNKNOWN_VOICE) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return ADVERB_PARTICIPLE_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    20      2      3      2        5
   * ----------------------------------------
   * Unused | Voice | Time | Lang | POS tag |
   * ----------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(lang_) << 5;
    ob |= static_cast<uint32_t>(time_) << 7;
    ob |= static_cast<uint32_t>(voice_) << 10;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    20      2      3      2        5
   * ----------------------------------------
   * Unused | Voice | Time | Lang | POS tag |
   * ----------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    lang_ = static_cast<Lang>((ib & 0x60) >> 5);
    time_ = static_cast<Time>((ib & 0x0380) >> 7);
    voice_ = static_cast<Voice>((ib & 0x0c00) >> 10);
  }

  Lang  lang_;
  Time  time_;
  Voice voice_;
};

/// Pronoun predicative class definition.
struct PronounPredicative : public RussianPos {
  PronounPredicative()
    : number_(UNKNOUN_NUMBER)
    , lang_(NORMAL_LANG)
    , case_(UNKNOWN_CASE) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return PRONOUN_PREDICATIVE_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    19     4      2       2        5
   * -----------------------------------------
   * Unused | Case | Lang | Number | POS tag |
   * -----------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(lang_) << 7;
    ob |= static_cast<uint32_t>(case_) << 9;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    19     4      2       2        5
   * -----------------------------------------
   * Unused | Case | Lang | Number | POS tag |
   * -----------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_ = static_cast<Number>((ib & 0x0060) >> 5);
    lang_ = static_cast<Lang>((ib & 0x0180) >> 7);
    case_ = static_cast<Case>((ib & 0x1e00) >> 9);
  }

  Number number_;
  Lang   lang_;
  Case   case_;
};

/// Pronoun adjective class definition.
struct PronounAdjective : public RussianPos {
  PronounAdjective()
    : number_(UNKNOUN_NUMBER)
    , lang_(NORMAL_LANG)
    , gender_(UNKNOWN_GENDER)
    , case_(UNKNOWN_CASE)
    , animation_(UNKNOWN_ANIMATION) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return PRONOUN_ADJECTIVE_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    14        2         4       3       2       2        5
   * --------------------------------------------------------------- 
   * Unused | Animation  | Case | Gender | Lang | Number | POS tag |
   * ---------------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(lang_) << 7;
    ob |= static_cast<uint32_t>(gender_) << 9;
    ob |= static_cast<uint32_t>(case_) << 12;
    ob |= static_cast<uint32_t>(animation_) << 16;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    14        2         4       3       2       2        5
   * --------------------------------------------------------------- 
   * Unused | Animation  | Case | Gender | Lang | Number | POS tag |
   * ---------------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_ = static_cast<Number>((ib & 0x0060) >> 5);
    lang_ = static_cast<Lang>((ib & 0x0180) >> 7);
    gender_ = static_cast<Gender>((ib & 0x0e00) >> 9);
    case_ = static_cast<Case>((ib & 0xf000) >> 12);
    animation_ = static_cast<Animation>((ib & 0x030000) >> 16);
  }

  Number    number_;
  Lang      lang_;
  Gender    gender_;
  Case      case_;
  Animation animation_;
};

/// Numeral (Quatitative) class definition.
struct NumeralQuantitative : public RussianPos {
  NumeralQuantitative()
    : lang_(NORMAL_LANG)
    , gender_(UNKNOWN_GENDER)
    , case_(UNKNOWN_CASE) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return NUMERAL_QUANTITATIVE_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    18     4       3       2        5
   * -----------------------------------------
   * Unused | Case | Gender | Lang | POS tag |
   * -----------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(lang_) << 5;
    ob |= static_cast<uint32_t>(gender_) << 7;
    ob |= static_cast<uint32_t>(case_) << 10;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    18     4       3       2        5
   * -----------------------------------------
   * Unused | Case | Gender | Lang | POS tag |
   * -----------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    lang_ = static_cast<Lang>((ib & 0x60) >> 5);
    gender_ = static_cast<Gender>((ib & 0x0380) >> 7);
    case_ = static_cast<Case>((ib & 0x3c00) >> 10);
  }

  Lang   lang_;
  Gender gender_;
  Case   case_;
};

/// Numeral (ordinal) class definition.
struct NumeralOrdinal : public RussianPos {
  /// Default initialization.
  NumeralOrdinal()
    : number_(UNKNOUN_NUMBER)
    , lang_(NORMAL_LANG)
    , gender_(UNKNOWN_GENDER)
    , case_(UNKNOWN_CASE)
    , animation_(UNKNOWN_ANIMATION) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return NUMERAL_ORDINAL_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    14        2         4       3       2       2        5
   * --------------------------------------------------------------- 
   * Unused | Animation  | Case | Gender | Lang | Number | POS tag |
   * ---------------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(number_) << 5;
    ob |= static_cast<uint32_t>(lang_) << 7;
    ob |= static_cast<uint32_t>(gender_) << 9;
    ob |= static_cast<uint32_t>(case_) << 12;
    ob |= static_cast<uint32_t>(animation_) << 16;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    14        2         4       3       2       2        5
   * --------------------------------------------------------------- 
   * Unused | Animation  | Case | Gender | Lang | Number | POS tag |
   * ---------------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    number_ = static_cast<Number>((ib & 0x0060) >> 5);
    lang_ = static_cast<Lang>((ib & 0x0180) >> 7);
    gender_ = static_cast<Gender>((ib & 0x0e00) >> 9);
    case_ = static_cast<Case>((ib & 0xf000) >> 12);
    animation_ = static_cast<Animation>((ib & 0x030000) >> 16);
  }

  Number    number_;
  Lang      lang_;
  Gender    gender_;
  Case      case_;
  Animation animation_;
};

/// Adverb class definition.
struct Adverb : public RussianPos {
  Adverb()
    : lang_(NORMAL_LANG)
    , relativity_(false)
    , quastionarity_(false)
    , brevity_(false) {
  }

  /// Get part of speech tag.
  PosTag GetPosTag() const { return ADVERB_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    22        1          1             1         2       5
   * ---------------------------------------------------------------
   * Unused | Brevity | Quastinality | Relativity | Lang | POS tag |
   * ---------------------------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(lang_) << 5;
    ob |= static_cast<uint32_t>(relativity_) << 7;
    ob |= static_cast<uint32_t>(quastionarity_) << 8;
    ob |= static_cast<uint32_t>(brevity_) << 9;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    22        1          1             1         2       5
   * ---------------------------------------------------------------
   * Unused | Brevity | Quastinality | Relativity | Lang | POS tag |
   * ---------------------------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    lang_ = static_cast<Lang>((ib & 0x60) >> 5);
    relativity_ = static_cast<bool>((ib & 0x80) >> 7);
    quastionarity_ = static_cast<bool>((ib & 0x0100) >> 8);
    brevity_ = static_cast<bool>((ib & 0x0200) >> 9);
  }

  Lang lang_;
  bool relativity_;    ///< Относительность.
  bool quastionarity_; ///< Вопросительность.
  bool brevity_;       ///< Краткость.
};

/// Predicate class definition.
struct Predicate : public RussianPos {
  Predicate()
    : lang_(NORMAL_LANG)
    , time_(UNKNOWN_TIME)
    , unchanged_(false) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return PREDICATE_PS; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    21        1        3      2       5
   * --------------------------------------------
   * Unused | Unchanged | Time | Lang | POS tag |
   * --------------------------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(lang_) << 5;
    ob |= static_cast<uint32_t>(time_) << 7;
    ob |= static_cast<uint32_t>(unchanged_) << 10;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    21        1        3      2       5
   * --------------------------------------------
   * Unused | Unchanged | Time | Lang | POS tag |
   * --------------------------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    lang_ = static_cast<Lang>((ib & 0x60) >> 5);
    time_ = static_cast<Time>((ib & 0x0380) >> 7);
    unchanged_ = static_cast<bool>((ib & 0x0400) >> 10);
  }

  Lang  lang_;
  Time  time_;
  bool  unchanged_; ///< Неизменяемость.
};

/// Temlate class for poor pos.
template <RussianPos::PosTag pos>
struct PoorPos : public RussianPos {
  PoorPos() : lang_(NORMAL_LANG) {}

  /// Get part of speech tag.
  PosTag GetPosTag() const { return pos; }

  /**
   * \brief Serialization implementaion.
   *
   * Binary map of the object:
   *    25      2       5
   * -------------------------
   * Unused | Lang | POS tag |
   * -------------------------
   *
   * \param[out] ob The buffer to write to.
   */
  void Serialize(uint32_t& ob) const {
    ob |= static_cast<uint32_t>(lang_) << 5;
  }

  /**
   * \brief Desirialization implementaion.
   *
   * Binary map of the object:
   *    25      2       5
   * -------------------------
   * Unused | Lang | POS tag |
   * -------------------------
   *
   * \param ib The buffer to write to.
   */
  void Deserialize(const uint32_t& ib) {
    lang_ = static_cast<Lang>((ib & 0x60) >> 5);
  }

  Lang lang_;
};

/// Preposion type.
typedef PoorPos<RussianPos::PREPOSITION_PS> Preposition;

/// Conjuction type.
typedef PoorPos<RussianPos::CONJUCTION_PS> Conjuction;

/// Interjection type.
typedef PoorPos<RussianPos::INTERJECTION_PS> Interjection;

/// Particle type.
typedef PoorPos<RussianPos::PARTICLE_PS> Particle;

/// Introductionary word type.
typedef PoorPos<RussianPos::INTRODUCTORY_WORD_PS> IntroductionaryWord;

/// Serialization implementation.
struct PosSerializer {
  /**
   * \brief Serialization.
   *
   * \param       pos pointer to POS class object.
   * \param[out]  ob  the buffer to write to.
   */
  static void Serialize(RussianPos::Ptr pos, uint32_t& ob) {
    ob = 0;
    RussianPos::WritePosSign(pos->GetPosTag(), ob);
    pos->Serialize(ob);
  }

  /**
   * \brief Deserialization.
   *
   * \param ob  the buffer to read from.
   */
  static RussianPos::Ptr Deserialize(const uint32_t& ob) {
    RussianPos::PosTag pos_tag = RussianPos::ReadPosSign(ob);
    RussianPos::Ptr pos;
    switch (pos_tag) {
      case RussianPos::NOUN_PS:
        pos = boost::make_shared<Noun>();
        break;
      case RussianPos::ADJECTIVE_PS:
        pos = boost::make_shared<Adjective>();
        break;
      case RussianPos::PRONOUN_NOUN_PS:
        pos = boost::make_shared<PronounNoun>();
        break;
      case RussianPos::VERB_PS:
        pos = boost::make_shared<Verb>();
        break;
      case RussianPos::ADVERB_PARTICIPLE_PS:
        pos = boost::make_shared<AdverbParticiple>();
        break;
      case RussianPos::PARTICIPLE_PS:
        pos = boost::make_shared<Participle>();
        break;
      case RussianPos::PRONOUN_PREDICATIVE_PS:
        pos = boost::make_shared<PronounPredicative>();
        break;
      case RussianPos::PRONOUN_ADJECTIVE_PS:
        pos = boost::make_shared<PronounAdjective>();
        break;
      case RussianPos::NUMERAL_QUANTITATIVE_PS:
        pos = boost::make_shared<NumeralQuantitative>();
        break;
      case RussianPos::NUMERAL_ORDINAL_PS:
        pos = boost::make_shared<NumeralOrdinal>();
        break;
      case RussianPos::ADVERB_PS:
        pos = boost::make_shared<Adverb>();
        break;
      case RussianPos::PREDICATE_PS:
        pos = boost::make_shared<Predicate>();
        break;
      case RussianPos::PREPOSITION_PS:
        pos = boost::make_shared<Preposition>();
        break;
      case RussianPos::CONJUCTION_PS:
        pos = boost::make_shared<Conjuction>();
        break;
      case RussianPos::INTERJECTION_PS:
        pos = boost::make_shared<Interjection>();
        break;
      case RussianPos::PARTICLE_PS:
        pos = boost::make_shared<Particle>();
        break;
      case RussianPos::INTRODUCTORY_WORD_PS:
        pos = boost::make_shared<IntroductionaryWord>();
        break;

      default:
        assert(false and "incorrect pos value");
    }

    pos->Deserialize(ob);
    return pos;
  }
};

}}} // namespace strutext, morpho, russian.
