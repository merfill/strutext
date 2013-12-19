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
 * \brief  Aho-Corasick trie unit test.
 * \author Vladimir Lapshin.
 */

#include <stdint.h>

#include <string>
#include <sstream>

#include <boost/test/unit_test.hpp>

#include "aho_corasick.h"
#include "ac_iterator.h"
#include "flex_transitions.h"
#include "serializer.h"

namespace {

// Type definitions.
typedef strutext::automata::FlexTransitions<char>                                 Trans;
typedef strutext::automata::AhoCorasickTrie<Trans, uint64_t>                      AcTrie;
typedef strutext::automata::FailMoveGenerator<AcTrie>                             AcTrieGenerator;
typedef strutext::automata::AcProcessor<AcTrie>                                   AcTrieProcessor;
typedef strutext::automata::AcSerializer<AcTrie>                                  AcSerializer;
typedef strutext::automata::AcSymbolIterator<AcTrie, std::string::const_iterator> AcIterator;
typedef strutext::automata::AcChainIterator<AcTrie, std::string::const_iterator>  ChainIterator;

// Utilities.
struct TrieUtils {
  // Addin a chain to the trie.
  static void AddChainToTrie(const std::string& chain, AcTrie::Attribute attr, AcTrie& trie) {
    trie.AddChain(chain.begin(), chain.end(), attr);
  }
};

}  // namespace.

BOOST_AUTO_TEST_CASE(Automata_AcTrie_Search) {
  // Create the trie and add chains there.
  AcTrie trie;
  TrieUtils::AddChainToTrie("abcde", 1, trie);
  TrieUtils::AddChainToTrie("ab", 2, trie);
  TrieUtils::AddChainToTrie("abc", 3, trie);
  TrieUtils::AddChainToTrie("abcd", 4, trie);
  TrieUtils::AddChainToTrie("cde", 5, trie);

  // Generate fail moves.
  AcTrieGenerator::Generate(trie);

  // Find chains.
  std::string text = "cdeabcde";
  AcTrieProcessor searcher(trie);
  strutext::automata::StateId state = strutext::automata::kStartState;
  for (size_t i = 0; i < text.length(); ++i) {
    state = searcher.Move(state, text[i]);
    const AcTrie::AttributeList& st_attrs = trie.GetStateAttributes(state);
    switch (i) {
      case 0: case 1: case 3: // There must be found nothing.
        BOOST_CHECK(st_attrs.size() == 0);
        break;
      case 2: // "cde" must be found.
        BOOST_CHECK(st_attrs.size() == 1);
        for (size_t j = 0; j < st_attrs.size(); ++j) {
          BOOST_CHECK(st_attrs[j] == 5);
        }
        break;
      case 4: // "ab" must be found.
        BOOST_CHECK(st_attrs.size() == 1);
        for (size_t j = 0; j < st_attrs.size(); ++j) {
          BOOST_CHECK(st_attrs[j] == 2);
        }
        break;
      case 5: // "abc" must be found.
        BOOST_CHECK(st_attrs.size() == 1);
        for (size_t j = 0; j < st_attrs.size(); ++j) {
          BOOST_CHECK(st_attrs[j] == 3);
        }
        break;
      case 6: // "abcd" must be found.
        BOOST_CHECK(st_attrs.size() == 1);
        for (size_t j = 0; j < st_attrs.size(); ++j) {
          BOOST_CHECK(st_attrs[j] == 4);
        }
        break;
      case 7: // "cde" and "abcde" must be found.
        BOOST_CHECK(st_attrs.size() == 2);
        for (size_t j = 0; j < st_attrs.size(); ++j) {
          BOOST_CHECK(st_attrs[j] == 1 or st_attrs[j] == 5);
        }
        break;
      default:
        BOOST_CHECK(false);
        break;
    }
  }
}

BOOST_AUTO_TEST_CASE(Automata_AcTrie_Serialize) {
  // Create the trie and add chains there.
  AcTrie trie;
  TrieUtils::AddChainToTrie("he", 1, trie);
  TrieUtils::AddChainToTrie("hers", 2, trie);
  TrieUtils::AddChainToTrie("his", 3, trie);
  TrieUtils::AddChainToTrie("she", 4, trie);

  // Generate fail transitions.
  AcTrieGenerator::Generate(trie);

  // Stream to serialization.
  std::stringstream ss;

  // Serialize the trie.
  AcSerializer::Serialize(trie, ss);

  // Deserialize the trie.
  AcTrie trie1;
  AcSerializer::Deserialize(trie1, ss);

  // Search chains in the text.
  std::string text = "ushers";
  AcTrieProcessor searcher(trie1);
  strutext::automata::StateId state = strutext::automata::kStartState;
  for (size_t i = 0; i < text.length(); ++i) {
    state = searcher.Move(state, text[i]);
    const AcTrie::AttributeList& st_attrs = trie.GetStateAttributes(state);
    switch (i) {
      case 0: case 1: case 2: case 4: // Nothing should be found.
        BOOST_CHECK(st_attrs.size() == 0);
        break;
      case 3: // "she" and "he" must be found.
        BOOST_CHECK(st_attrs.size() == 2);
        for (size_t j = 0; j < st_attrs.size(); ++j) {
          BOOST_CHECK(st_attrs[j] == 1 or st_attrs[j] == 4);
        }
        break;
      case 5: // "hers" must be found.
        BOOST_CHECK(st_attrs.size() == 1);
        for (size_t j = 0; j < st_attrs.size(); ++j) {
          BOOST_CHECK(st_attrs[j] == 2);
        }
        break;
      default:
        BOOST_CHECK(false);
        break;
    }
  }
}

// AC iterator simple search test.
BOOST_AUTO_TEST_CASE(Automata_AcIterator_Search) {
  // Create the trie and add chains there.
  AcTrie trie;
  TrieUtils::AddChainToTrie("he", 1, trie);
  TrieUtils::AddChainToTrie("hers", 2, trie);
  TrieUtils::AddChainToTrie("his", 3, trie);
  TrieUtils::AddChainToTrie("she", 4, trie);

  // Generate fail transitions.
  AcTrieGenerator::Generate(trie);

  // Find chains.
  std::string text = "ushers";
  size_t pos = 0;
  for (AcIterator it(text.begin(), text.end(), trie); it != AcIterator(); ++it, ++pos) {
    switch (pos) {
      case 0: case 1: case 2: case 4: // Nothing should be found.
        BOOST_CHECK((*it).size() == 0);
        break;
      case 3: // "she" and "he" should be found.
        BOOST_CHECK((*it).size() == 2);
        for (size_t i = 0; i < (*it).size(); ++i) {
          BOOST_CHECK((*it)[i] == 1 or (*it)[i] == 4);
        }
        break;
      case 5: // "hers" should be found.
        BOOST_CHECK((*it).size() == 1);
        for (size_t i = 0; i < (*it).size(); ++i) {
          BOOST_CHECK((*it)[i] == 2);
        }
        break;
      default:
        BOOST_CHECK(false);
        break;
    }
  }
}

// AC chain iterator simple search test.
BOOST_AUTO_TEST_CASE(Automata_ChainIterator_Search) {
  // Create the trie and add chains there.
  AcTrie trie;
  TrieUtils::AddChainToTrie("he", 1, trie);
  TrieUtils::AddChainToTrie("hers", 2, trie);
  TrieUtils::AddChainToTrie("his", 3, trie);
  TrieUtils::AddChainToTrie("she", 4, trie);

  // Generate fail transitions.
  AcTrieGenerator::Generate(trie);

  // Chains find flags.
  bool he_found = false, she_found = false, hers_found = false;

  // Find chains.
  std::string text = "ushers";
  for (ChainIterator it(text.begin(), text.end(), trie); it != ChainIterator(); ++it) {
    switch (*it) {
      case 1: case 4: // "she" and "he" found, it must be position 4.
        BOOST_CHECK(it.GetPos() == 4);
        if (*it == 1) {
          he_found = true;
        } else if (*it == 4) {
          she_found = true;
        }
        break;
      case 2: // "hers" found, it must be position 6.
        BOOST_CHECK(it.GetPos() == 6);
        hers_found = true;
        break;
      default:
        BOOST_CHECK(false);
        break;
    }
  }
  BOOST_CHECK(he_found and she_found and hers_found);
}
