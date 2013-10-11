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
 * \brief  Russian morpho model unit test.
 * \author Vladimir Lapshin.
 */

#include <memory>

#include <boost/test/unit_test.hpp>

#include "rus_model.h"
namespace m = strutext::morpho;
namespace rm = strutext::morpho::russian;

// Noun test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_Noun) {
  m::RussianPos::Ptr noun_ptr = std::make_shared<rm::Noun>();
  rm::Noun* noun = static_cast<rm::Noun*>(noun_ptr.get());
  noun->number_   = m::RussianPos::PLURAL_NUMBER;
  noun->lang_     = m::RussianPos::ARCHAIZM_LANG;
  noun->gender_   = m::RussianPos::FEMININE_GENDER;
  noun->case_     = m::RussianPos::PREPOSITIONAL_CASE;
  noun->entity_   = m::RussianPos::MIDDLE_NAME_ENTITY;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(noun_ptr, ob);
  m::RussianPos::Ptr noun1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Noun* noun1 = static_cast<rm::Noun*>(noun1_ptr.get());
  BOOST_CHECK_EQUAL(noun1->GetPosTag(), m::RussianPos::NOUN_PS);
  BOOST_CHECK_EQUAL(noun1->number_,     m::RussianPos::PLURAL_NUMBER);
  BOOST_CHECK_EQUAL(noun1->lang_,       m::RussianPos::ARCHAIZM_LANG);
  BOOST_CHECK_EQUAL(noun1->gender_,     m::RussianPos::FEMININE_GENDER);
  BOOST_CHECK_EQUAL(noun1->case_,       m::RussianPos::PREPOSITIONAL_CASE);
  BOOST_CHECK_EQUAL(noun1->entity_,     m::RussianPos::MIDDLE_NAME_ENTITY);
}

// Adjective test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_Adjective) {
  m::RussianPos::Ptr adj_ptr = std::make_shared<rm::Adjective>();
  rm::Adjective* adj = static_cast<rm::Adjective*>(adj_ptr.get());
  adj->number_    = m::RussianPos::PLURAL_NUMBER;
  adj->lang_      = m::RussianPos::NORMAL_LANG;
  adj->gender_    = m::RussianPos::FEMININE_GENDER;
  adj->case_      = m::RussianPos::GENITIVE_CASE;
  adj->animation_ = m::RussianPos::INANIMATE_ANIMATION;
  adj->brevity_   = true;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(adj_ptr, ob);
  m::RussianPos::Ptr adj1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Adjective* adj1 = static_cast<rm::Adjective*>(adj1_ptr.get());
  BOOST_CHECK_EQUAL(adj1->GetPosTag(),  m::RussianPos::ADJECTIVE_PS);
  BOOST_CHECK_EQUAL(adj1->number_,      m::RussianPos::PLURAL_NUMBER);
  BOOST_CHECK_EQUAL(adj1->lang_,        m::RussianPos::NORMAL_LANG);
  BOOST_CHECK_EQUAL(adj1->gender_,      m::RussianPos::FEMININE_GENDER);
  BOOST_CHECK_EQUAL(adj1->case_,        m::RussianPos::GENITIVE_CASE);
  BOOST_CHECK_EQUAL(adj1->animation_,   m::RussianPos::INANIMATE_ANIMATION);
  BOOST_CHECK_EQUAL(adj1->brevity_,     true);
}

// PronounNoun test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_PronounNoun) {
  m::RussianPos::Ptr pn_ptr = std::make_shared<rm::PronounNoun>();
  rm::PronounNoun* pn = static_cast<rm::PronounNoun*>(pn_ptr.get());
  pn->number_  = m::RussianPos::PLURAL_NUMBER;
  pn->lang_    = m::RussianPos::SLANG_LANG;
  pn->gender_  = m::RussianPos::NEUTER_GENDER;
  pn->case_    = m::RussianPos::INSTRUMENTAL_CASE;
  pn->person_  = m::RussianPos::SECOND_PERSON;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pn_ptr, ob);
  m::RussianPos::Ptr pn1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::PronounNoun* pn1 = static_cast<rm::PronounNoun*>(pn1_ptr.get());
  BOOST_CHECK_EQUAL(pn1->GetPosTag(), m::RussianPos::PRONOUN_NOUN_PS);
  BOOST_CHECK_EQUAL(pn1->number_,     m::RussianPos::PLURAL_NUMBER);
  BOOST_CHECK_EQUAL(pn1->lang_,       m::RussianPos::SLANG_LANG);
  BOOST_CHECK_EQUAL(pn1->gender_,     m::RussianPos::NEUTER_GENDER);
  BOOST_CHECK_EQUAL(pn1->case_,       m::RussianPos::INSTRUMENTAL_CASE);
  BOOST_CHECK_EQUAL(pn1->person_,     m::RussianPos::SECOND_PERSON);
}

// Verb test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_Verb) {
  m::RussianPos::Ptr verb_ptr = std::make_shared<rm::Verb>();
  rm::Verb* verb = static_cast<rm::Verb*>(verb_ptr.get());
  verb->number_     = m::RussianPos::PLURAL_NUMBER;
  verb->lang_       = m::RussianPos::INFORMAL_LANG;
  verb->time_       = m::RussianPos::FUTURE_TIME;
  verb->voice_      = m::RussianPos::ACTIVE_VOICE;
  verb->person_     = m::RussianPos::FIRST_PERSON;
  verb->impersonal_ = false;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(verb_ptr, ob);
  m::RussianPos::Ptr verb1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Verb* verb1 = static_cast<rm::Verb*>(verb1_ptr.get());
  BOOST_CHECK_EQUAL(verb1->GetPosTag(), m::RussianPos::VERB_PS);
  BOOST_CHECK_EQUAL(verb1->number_,     m::RussianPos::PLURAL_NUMBER);
  BOOST_CHECK_EQUAL(verb1->lang_,       m::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(verb1->time_,       m::RussianPos::FUTURE_TIME);
  BOOST_CHECK_EQUAL(verb1->voice_,      m::RussianPos::ACTIVE_VOICE);
  BOOST_CHECK_EQUAL(verb1->person_,     m::RussianPos::FIRST_PERSON);
  BOOST_CHECK_EQUAL(verb1->impersonal_, false);
}

// Participle test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_Participle) {
  m::RussianPos::Ptr pt_ptr = std::make_shared<rm::Participle>();
  rm::Participle* pt = static_cast<rm::Participle*>(pt_ptr.get());
  pt->number_     = m::RussianPos::SINGULAR_NUMBER;
  pt->lang_       = m::RussianPos::NORMAL_LANG;
  pt->time_       = m::RussianPos::FUTURE_TIME;
  pt->voice_      = m::RussianPos::ACTIVE_VOICE;
  pt->case_       = m::RussianPos::ACCUSATIVE_CASE;
  pt->gender_     = m::RussianPos::MASCULINE_GENDER;
  pt->animation_  = m::RussianPos::ANIMATE_ANIMATION;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  m::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Participle* pt1 = static_cast<rm::Participle*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), m::RussianPos::PARTICIPLE_PS);
  BOOST_CHECK_EQUAL(pt1->number_,     m::RussianPos::SINGULAR_NUMBER);
  BOOST_CHECK_EQUAL(pt1->lang_,       m::RussianPos::NORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->time_,       m::RussianPos::FUTURE_TIME);
  BOOST_CHECK_EQUAL(pt1->voice_,      m::RussianPos::ACTIVE_VOICE);
  BOOST_CHECK_EQUAL(pt1->case_,       m::RussianPos::ACCUSATIVE_CASE);
  BOOST_CHECK_EQUAL(pt1->gender_,     m::RussianPos::MASCULINE_GENDER);
  BOOST_CHECK_EQUAL(pt1->animation_,  m::RussianPos::ANIMATE_ANIMATION);
}

// AdverbParticiple test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_AdverbParticiple) {
  m::RussianPos::Ptr pt_ptr = std::make_shared<rm::AdverbParticiple>();
  rm::AdverbParticiple* pt = static_cast<rm::AdverbParticiple*>(pt_ptr.get());
  pt->lang_       = m::RussianPos::NORMAL_LANG;
  pt->time_       = m::RussianPos::FUTURE_TIME;
  pt->voice_      = m::RussianPos::ACTIVE_VOICE;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  m::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::AdverbParticiple* pt1 = static_cast<rm::AdverbParticiple*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), m::RussianPos::ADVERB_PARTICIPLE_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,       m::RussianPos::NORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->time_,       m::RussianPos::FUTURE_TIME);
  BOOST_CHECK_EQUAL(pt1->voice_,      m::RussianPos::ACTIVE_VOICE);
}

// PronounPredicative test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_PronounPredicative) {
  m::RussianPos::Ptr pt_ptr = std::make_shared<rm::PronounPredicative>();
  rm::PronounPredicative* pt = static_cast<rm::PronounPredicative*>(pt_ptr.get());
  pt->number_    = m::RussianPos::PLURAL_NUMBER;
  pt->lang_      = m::RussianPos::NORMAL_LANG;
  pt->case_      = m::RussianPos::DATIVE_CASE;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  m::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::PronounPredicative* pt1 = static_cast<rm::PronounPredicative*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), m::RussianPos::PRONOUN_PREDICATIVE_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,       m::RussianPos::NORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->number_,     m::RussianPos::PLURAL_NUMBER);
  BOOST_CHECK_EQUAL(pt1->case_,       m::RussianPos::DATIVE_CASE);
}

// PronounAdjective test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_PronounAdjective) {
  m::RussianPos::Ptr pt_ptr = std::make_shared<rm::PronounAdjective>();
  rm::PronounAdjective* pt = static_cast<rm::PronounAdjective*>(pt_ptr.get());
  pt->number_     = m::RussianPos::SINGULAR_NUMBER;
  pt->lang_       = m::RussianPos::INFORMAL_LANG;
  pt->gender_     = m::RussianPos::MASCULINE_GENDER;
  pt->case_       = m::RussianPos::VOCATIVE_CASE;
  pt->animation_  = m::RussianPos::ANIMATE_ANIMATION;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  m::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::PronounAdjective* pt1 = static_cast<rm::PronounAdjective*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), m::RussianPos::PRONOUN_ADJECTIVE_PS);
  BOOST_CHECK_EQUAL(pt1->number_,     m::RussianPos::SINGULAR_NUMBER);
  BOOST_CHECK_EQUAL(pt1->lang_,       m::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->gender_,     m::RussianPos::MASCULINE_GENDER);
  BOOST_CHECK_EQUAL(pt1->case_,       m::RussianPos::VOCATIVE_CASE);
  BOOST_CHECK_EQUAL(pt1->animation_,  m::RussianPos::ANIMATE_ANIMATION);
}

// NumeralQuantitative test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_NumeralQuantitative) {
  m::RussianPos::Ptr pt_ptr = std::make_shared<rm::NumeralQuantitative>();
  rm::NumeralQuantitative* pt = static_cast<rm::NumeralQuantitative*>(pt_ptr.get());
  pt->lang_       = m::RussianPos::INFORMAL_LANG;
  pt->gender_     = m::RussianPos::MASCULINE_GENDER;
  pt->case_       = m::RussianPos::VOCATIVE_CASE;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  m::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::NumeralQuantitative* pt1 = static_cast<rm::NumeralQuantitative*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), m::RussianPos::NUMERAL_QUANTITATIVE_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,       m::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->gender_,     m::RussianPos::MASCULINE_GENDER);
  BOOST_CHECK_EQUAL(pt1->case_,       m::RussianPos::VOCATIVE_CASE);
}

// NumeralOrdinal test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_NumeralOrdinal) {
  m::RussianPos::Ptr pt_ptr = std::make_shared<rm::NumeralOrdinal>();
  rm::NumeralOrdinal* pt = static_cast<rm::NumeralOrdinal*>(pt_ptr.get());
  pt->number_     = m::RussianPos::SINGULAR_NUMBER;
  pt->lang_       = m::RussianPos::INFORMAL_LANG;
  pt->gender_     = m::RussianPos::MASCULINE_GENDER;
  pt->case_       = m::RussianPos::VOCATIVE_CASE;
  pt->animation_  = m::RussianPos::ANIMATE_ANIMATION;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  m::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::NumeralOrdinal* pt1 = static_cast<rm::NumeralOrdinal*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), m::RussianPos::NUMERAL_ORDINAL_PS);
  BOOST_CHECK_EQUAL(pt1->number_,     m::RussianPos::SINGULAR_NUMBER);
  BOOST_CHECK_EQUAL(pt1->lang_,       m::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->gender_,     m::RussianPos::MASCULINE_GENDER);
  BOOST_CHECK_EQUAL(pt1->case_,       m::RussianPos::VOCATIVE_CASE);
  BOOST_CHECK_EQUAL(pt1->animation_,  m::RussianPos::ANIMATE_ANIMATION);
}

// Adverb test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_Adverb) {
  m::RussianPos::Ptr pt_ptr = std::make_shared<rm::Adverb>();
  rm::Adverb* pt = static_cast<rm::Adverb*>(pt_ptr.get());
  pt->lang_           = m::RussianPos::INFORMAL_LANG;
  pt->relativity_     = true;
  pt->quastionarity_  = false;
  pt->brevity_        = true;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  m::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Adverb* pt1 = static_cast<rm::Adverb*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(),     m::RussianPos::ADVERB_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,           m::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->relativity_,     true);
  BOOST_CHECK_EQUAL(pt1->quastionarity_,  false);
  BOOST_CHECK_EQUAL(pt1->brevity_,        true);
}

// Predicate test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_Predicate) {
  m::RussianPos::Ptr pt_ptr = std::make_shared<rm::Predicate>();
  rm::Predicate* pt = static_cast<rm::Predicate*>(pt_ptr.get());
  pt->lang_       = m::RussianPos::INFORMAL_LANG;
  pt->time_       = m::RussianPos::FUTURE_TIME;
  pt->unchanged_  = true;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  m::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Predicate* pt1 = static_cast<rm::Predicate*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(),   m::RussianPos::PREDICATE_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,         m::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->time_,         m::RussianPos::FUTURE_TIME);
  BOOST_CHECK_EQUAL(pt1->unchanged_,    true);
}

// Preposition test.
BOOST_AUTO_TEST_CASE(Morpho_RussianModel_Preposition) {
  m::RussianPos::Ptr pt_ptr = std::make_shared<rm::Preposition>();
  rm::Preposition* pt = static_cast<rm::Preposition*>(pt_ptr.get());
  pt->lang_       = m::RussianPos::NORMAL_LANG;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  m::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Preposition* pt1 = static_cast<rm::Preposition*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(),   m::RussianPos::PREPOSITION_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,         m::RussianPos::NORMAL_LANG);
}
