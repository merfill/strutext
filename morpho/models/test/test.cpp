/** Copyright &copy; 2011-2012, Factor.
 * \file   test.cpp
 * \brief  Russian morpho model unit test.
 * \author Vladimir Lapshin.
 * \date   27.05.2012
 */

#define BOOST_TEST_MODULE rusmodel
#define BOOST_AUTO_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <rus_model.h>
namespace rm = factor::morpho::russian;

// Noun test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_Noun) {
  rm::RussianPos::Ptr noun_ptr(new rm::Noun());
  rm::Noun* noun = static_cast<rm::Noun*>(noun_ptr.get());
  noun->number_   = rm::RussianPos::PLURAL_NUMBER;
  noun->lang_     = rm::RussianPos::ARCHAIZM_LANG;
  noun->gender_   = rm::RussianPos::FEMININE_GENDER;
  noun->case_     = rm::RussianPos::PREPOSITIONAL_CASE;
  noun->entity_   = rm::RussianPos::MIDDLE_NAME_ENTITY;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(noun_ptr, ob);
  rm::RussianPos::Ptr noun1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Noun* noun1 = static_cast<rm::Noun*>(noun1_ptr.get());
  BOOST_CHECK_EQUAL(noun1->GetPosTag(), rm::RussianPos::NOUN_PS);
  BOOST_CHECK_EQUAL(noun1->number_,     rm::RussianPos::PLURAL_NUMBER);
  BOOST_CHECK_EQUAL(noun1->lang_,       rm::RussianPos::ARCHAIZM_LANG);
  BOOST_CHECK_EQUAL(noun1->gender_,     rm::RussianPos::FEMININE_GENDER);
  BOOST_CHECK_EQUAL(noun1->case_,       rm::RussianPos::PREPOSITIONAL_CASE);
  BOOST_CHECK_EQUAL(noun1->entity_,     rm::RussianPos::MIDDLE_NAME_ENTITY);
}

// Adjective test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_Adjective) {
  rm::RussianPos::Ptr adj_ptr(new rm::Adjective());
  rm::Adjective* adj = static_cast<rm::Adjective*>(adj_ptr.get());
  adj->number_    = rm::RussianPos::PLURAL_NUMBER;
  adj->lang_      = rm::RussianPos::NORMAL_LANG;
  adj->gender_    = rm::RussianPos::FEMININE_GENDER;
  adj->case_      = rm::RussianPos::GENITIVE_CASE;
  adj->animation_ = rm::RussianPos::INANIMATE_ANIMATION;
  adj->brevity_   = true;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(adj_ptr, ob);
  rm::RussianPos::Ptr adj1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Adjective* adj1 = static_cast<rm::Adjective*>(adj1_ptr.get());
  BOOST_CHECK_EQUAL(adj1->GetPosTag(),  rm::RussianPos::ADJECTIVE_PS);
  BOOST_CHECK_EQUAL(adj1->number_,      rm::RussianPos::PLURAL_NUMBER);
  BOOST_CHECK_EQUAL(adj1->lang_,        rm::RussianPos::NORMAL_LANG);
  BOOST_CHECK_EQUAL(adj1->gender_,      rm::RussianPos::FEMININE_GENDER);
  BOOST_CHECK_EQUAL(adj1->case_,        rm::RussianPos::GENITIVE_CASE);
  BOOST_CHECK_EQUAL(adj1->animation_,   rm::RussianPos::INANIMATE_ANIMATION);
  BOOST_CHECK_EQUAL(adj1->brevity_,     true);
}

// PronounNoun test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_PronounNoun) {
  rm::RussianPos::Ptr pn_ptr(new rm::PronounNoun());
  rm::PronounNoun* pn = static_cast<rm::PronounNoun*>(pn_ptr.get());
  pn->number_  = rm::RussianPos::PLURAL_NUMBER;
  pn->lang_    = rm::RussianPos::SLANG_LANG;
  pn->gender_  = rm::RussianPos::NEUTER_GENDER;
  pn->case_    = rm::RussianPos::INSTRUMENTAL_CASE;
  pn->person_  = rm::RussianPos::SECOND_PERSON;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pn_ptr, ob);
  rm::RussianPos::Ptr pn1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::PronounNoun* pn1 = static_cast<rm::PronounNoun*>(pn1_ptr.get());
  BOOST_CHECK_EQUAL(pn1->GetPosTag(), rm::RussianPos::PRONOUN_NOUN_PS);
  BOOST_CHECK_EQUAL(pn1->number_,     rm::RussianPos::PLURAL_NUMBER);
  BOOST_CHECK_EQUAL(pn1->lang_,       rm::RussianPos::SLANG_LANG);
  BOOST_CHECK_EQUAL(pn1->gender_,     rm::RussianPos::NEUTER_GENDER);
  BOOST_CHECK_EQUAL(pn1->case_,       rm::RussianPos::INSTRUMENTAL_CASE);
  BOOST_CHECK_EQUAL(pn1->person_,     rm::RussianPos::SECOND_PERSON);
}

// Verb test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_Verb) {
  rm::RussianPos::Ptr verb_ptr(new rm::Verb());
  rm::Verb* verb = static_cast<rm::Verb*>(verb_ptr.get());
  verb->number_     = rm::RussianPos::PLURAL_NUMBER;
  verb->lang_       = rm::RussianPos::INFORMAL_LANG;
  verb->time_       = rm::RussianPos::FUTURE_TIME;
  verb->voice_      = rm::RussianPos::ACTIVE_VOICE;
  verb->person_     = rm::RussianPos::FIRST_PERSON;
  verb->impersonal_ = false;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(verb_ptr, ob);
  rm::RussianPos::Ptr verb1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Verb* verb1 = static_cast<rm::Verb*>(verb1_ptr.get());
  BOOST_CHECK_EQUAL(verb1->GetPosTag(), rm::RussianPos::VERB_PS);
  BOOST_CHECK_EQUAL(verb1->number_,     rm::RussianPos::PLURAL_NUMBER);
  BOOST_CHECK_EQUAL(verb1->lang_,       rm::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(verb1->time_,       rm::RussianPos::FUTURE_TIME);
  BOOST_CHECK_EQUAL(verb1->voice_,      rm::RussianPos::ACTIVE_VOICE);
  BOOST_CHECK_EQUAL(verb1->person_,     rm::RussianPos::FIRST_PERSON);
  BOOST_CHECK_EQUAL(verb1->impersonal_, false);
}

// Participle test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_Participle) {
  rm::RussianPos::Ptr pt_ptr(new rm::Participle());
  rm::Participle* pt = static_cast<rm::Participle*>(pt_ptr.get());
  pt->number_     = rm::RussianPos::SINGULAR_NUMBER;
  pt->lang_       = rm::RussianPos::NORMAL_LANG;
  pt->time_       = rm::RussianPos::FUTURE_TIME;
  pt->voice_      = rm::RussianPos::ACTIVE_VOICE;
  pt->case_       = rm::RussianPos::ACCUSATIVE_CASE;
  pt->gender_     = rm::RussianPos::MASCULINE_GENDER;
  pt->animation_  = rm::RussianPos::ANIMATE_ANIMATION;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  rm::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Participle* pt1 = static_cast<rm::Participle*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), rm::RussianPos::PARTICIPLE_PS);
  BOOST_CHECK_EQUAL(pt1->number_,     rm::RussianPos::SINGULAR_NUMBER);
  BOOST_CHECK_EQUAL(pt1->lang_,       rm::RussianPos::NORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->time_,       rm::RussianPos::FUTURE_TIME);
  BOOST_CHECK_EQUAL(pt1->voice_,      rm::RussianPos::ACTIVE_VOICE);
  BOOST_CHECK_EQUAL(pt1->case_,       rm::RussianPos::ACCUSATIVE_CASE);
  BOOST_CHECK_EQUAL(pt1->gender_,     rm::RussianPos::MASCULINE_GENDER);
  BOOST_CHECK_EQUAL(pt1->animation_,  rm::RussianPos::ANIMATE_ANIMATION);
}

// AdverbParticiple test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_AdverbParticiple) {
  rm::RussianPos::Ptr pt_ptr(new rm::AdverbParticiple());
  rm::AdverbParticiple* pt = static_cast<rm::AdverbParticiple*>(pt_ptr.get());
  pt->lang_       = rm::RussianPos::NORMAL_LANG;
  pt->time_       = rm::RussianPos::FUTURE_TIME;
  pt->voice_      = rm::RussianPos::ACTIVE_VOICE;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  rm::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::AdverbParticiple* pt1 = static_cast<rm::AdverbParticiple*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), rm::RussianPos::ADVERB_PARTICIPLE_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,       rm::RussianPos::NORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->time_,       rm::RussianPos::FUTURE_TIME);
  BOOST_CHECK_EQUAL(pt1->voice_,      rm::RussianPos::ACTIVE_VOICE);
}

// PronounPredicative test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_PronounPredicative) {
  rm::RussianPos::Ptr pt_ptr(new rm::PronounPredicative());
  rm::PronounPredicative* pt = static_cast<rm::PronounPredicative*>(pt_ptr.get());
  pt->number_    = rm::RussianPos::PLURAL_NUMBER;
  pt->lang_      = rm::RussianPos::NORMAL_LANG;
  pt->case_      = rm::RussianPos::DATIVE_CASE;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  rm::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::PronounPredicative* pt1 = static_cast<rm::PronounPredicative*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), rm::RussianPos::PRONOUN_PREDICATIVE_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,       rm::RussianPos::NORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->number_,     rm::RussianPos::PLURAL_NUMBER);
  BOOST_CHECK_EQUAL(pt1->case_,       rm::RussianPos::DATIVE_CASE);
}

// PronounAdjective test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_PronounAdjective) {
  rm::RussianPos::Ptr pt_ptr(new rm::PronounAdjective());
  rm::PronounAdjective* pt = static_cast<rm::PronounAdjective*>(pt_ptr.get());
  pt->number_     = rm::RussianPos::SINGULAR_NUMBER;
  pt->lang_       = rm::RussianPos::INFORMAL_LANG;
  pt->gender_     = rm::RussianPos::MASCULINE_GENDER;
  pt->case_       = rm::RussianPos::VOCATIVE_CASE;
  pt->animation_  = rm::RussianPos::ANIMATE_ANIMATION;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  rm::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::PronounAdjective* pt1 = static_cast<rm::PronounAdjective*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), rm::RussianPos::PRONOUN_ADJECTIVE_PS);
  BOOST_CHECK_EQUAL(pt1->number_,     rm::RussianPos::SINGULAR_NUMBER);
  BOOST_CHECK_EQUAL(pt1->lang_,       rm::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->gender_,     rm::RussianPos::MASCULINE_GENDER);
  BOOST_CHECK_EQUAL(pt1->case_,       rm::RussianPos::VOCATIVE_CASE);
  BOOST_CHECK_EQUAL(pt1->animation_,  rm::RussianPos::ANIMATE_ANIMATION);
}

// NumeralQuantitative test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_NumeralQuantitative) {
  rm::RussianPos::Ptr pt_ptr(new rm::NumeralQuantitative());
  rm::NumeralQuantitative* pt = static_cast<rm::NumeralQuantitative*>(pt_ptr.get());
  pt->lang_       = rm::RussianPos::INFORMAL_LANG;
  pt->gender_     = rm::RussianPos::MASCULINE_GENDER;
  pt->case_       = rm::RussianPos::VOCATIVE_CASE;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  rm::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::NumeralQuantitative* pt1 = static_cast<rm::NumeralQuantitative*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), rm::RussianPos::NUMERAL_QUANTITATIVE_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,       rm::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->gender_,     rm::RussianPos::MASCULINE_GENDER);
  BOOST_CHECK_EQUAL(pt1->case_,       rm::RussianPos::VOCATIVE_CASE);
}

// NumeralOrdinal test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_NumeralOrdinal) {
  rm::RussianPos::Ptr pt_ptr(new rm::NumeralOrdinal());
  rm::NumeralOrdinal* pt = static_cast<rm::NumeralOrdinal*>(pt_ptr.get());
  pt->number_     = rm::RussianPos::SINGULAR_NUMBER;
  pt->lang_       = rm::RussianPos::INFORMAL_LANG;
  pt->gender_     = rm::RussianPos::MASCULINE_GENDER;
  pt->case_       = rm::RussianPos::VOCATIVE_CASE;
  pt->animation_  = rm::RussianPos::ANIMATE_ANIMATION;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  rm::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::NumeralOrdinal* pt1 = static_cast<rm::NumeralOrdinal*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(), rm::RussianPos::NUMERAL_ORDINAL_PS);
  BOOST_CHECK_EQUAL(pt1->number_,     rm::RussianPos::SINGULAR_NUMBER);
  BOOST_CHECK_EQUAL(pt1->lang_,       rm::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->gender_,     rm::RussianPos::MASCULINE_GENDER);
  BOOST_CHECK_EQUAL(pt1->case_,       rm::RussianPos::VOCATIVE_CASE);
  BOOST_CHECK_EQUAL(pt1->animation_,  rm::RussianPos::ANIMATE_ANIMATION);
}

// Adverb test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_Adverb) {
  rm::RussianPos::Ptr pt_ptr(new rm::Adverb());
  rm::Adverb* pt = static_cast<rm::Adverb*>(pt_ptr.get());
  pt->lang_           = rm::RussianPos::INFORMAL_LANG;
  pt->relativity_     = true;
  pt->quastionarity_  = false;
  pt->brevity_        = true;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  rm::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Adverb* pt1 = static_cast<rm::Adverb*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(),     rm::RussianPos::ADVERB_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,           rm::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->relativity_,     true);
  BOOST_CHECK_EQUAL(pt1->quastionarity_,  false);
  BOOST_CHECK_EQUAL(pt1->brevity_,        true);
}

// Predicate test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_Predicate) {
  rm::RussianPos::Ptr pt_ptr(new rm::Predicate());
  rm::Predicate* pt = static_cast<rm::Predicate*>(pt_ptr.get());
  pt->lang_       = rm::RussianPos::INFORMAL_LANG;
  pt->time_       = rm::RussianPos::FUTURE_TIME;
  pt->unchanged_  = true;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  rm::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Predicate* pt1 = static_cast<rm::Predicate*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(),   rm::RussianPos::PREDICATE_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,         rm::RussianPos::INFORMAL_LANG);
  BOOST_CHECK_EQUAL(pt1->time_,         rm::RussianPos::FUTURE_TIME);
  BOOST_CHECK_EQUAL(pt1->unchanged_,    true);
}

// Preposition test.
BOOST_AUTO_TEST_CASE(Morpho_Russian_Preposition) {
  rm::RussianPos::Ptr pt_ptr(new rm::Preposition());
  rm::Preposition* pt = static_cast<rm::Preposition*>(pt_ptr.get());
  pt->lang_       = rm::RussianPos::NORMAL_LANG;

  uint32_t ob = 0;
  rm::PosSerializer::Serialize(pt_ptr, ob);
  rm::RussianPos::Ptr pt1_ptr = rm::PosSerializer::Deserialize(ob);

  rm::Preposition* pt1 = static_cast<rm::Preposition*>(pt1_ptr.get());
  BOOST_CHECK_EQUAL(pt1->GetPosTag(),   rm::RussianPos::PREPOSITION_PS);
  BOOST_CHECK_EQUAL(pt1->lang_,         rm::RussianPos::NORMAL_LANG);
}
