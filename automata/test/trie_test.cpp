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
 * \brief  Trie unit test.
 * \author Vladimir Lapshin.
 */

#include <cstdint>
#include <sstream>
#include <string>

#include <boost/test/unit_test.hpp>

#include "trie.h"
#include "flex_transitions.h"
#include "serializer.h"

namespace {

// Trie type definitions.
typedef strutext::automata::FlexTransitions<char>       Trans;
typedef strutext::automata::Trie<Trans, uint64_t>       FlexTrie;
typedef strutext::automata::AttrFsmSerializer<FlexTrie> Serializer;

// Utilities.
struct TrieUtils {
  // Search chain in the passed trie.
  static bool CheckCnainInTrie(const std::string& chain, FlexTrie::Attribute attr, const FlexTrie& trie) {
    strutext::automata::StateId state = strutext::automata::kStartState;
    for (auto it = chain.begin(); it != chain.end(); ++it) {
      state = trie.Go(state, *it);
      if (state == strutext::automata::kInvalidState) {
        return false;
      }
    }

    // Check if we are in the acceptable state.
    if (not trie.IsAcceptable(state)) {
      return false;
    }

    // Check is there the passed value in the attribute list.
    const FlexTrie::AttributeList& attrs = trie.GetStateAttributes(state);
    for (auto attribute : attrs) {
      if (attribute == attr) {
        return true;
      }
    }

    // The chain hasn't been found.
    return false;
  }

  // Adding a chain in the trie.
  static void AddChainToTrie(const std::string& chain, FlexTrie::Attribute attr, FlexTrie& trie) {
    trie.AddChain(chain.begin(), chain.end(), attr);
  }
};

}  // namespace.

// Simple work test.
BOOST_AUTO_TEST_CASE(Automata_Trie_SimpleCheck) {
  // Create trie and add chains into it.
  FlexTrie trie;
  TrieUtils::AddChainToTrie("hello", 1, trie);
  TrieUtils::AddChainToTrie("bye", 2, trie);

  // Check the chains are in the trie.
  BOOST_CHECK(TrieUtils::CheckCnainInTrie("hello", 1, trie));
  BOOST_CHECK(TrieUtils::CheckCnainInTrie("bye", 2, trie));
}

// Serialization test.
BOOST_AUTO_TEST_CASE(Automata_Trie_Serialize) {
  // Create trie and add chains into it.
  FlexTrie trie;
  TrieUtils::AddChainToTrie("hello", 1, trie);
  TrieUtils::AddChainToTrie("bye", 2, trie);

  // The stream to serialize chains.
  std::stringstream ss;

  // Serialize the trie.
  Serializer::Serialize(trie, ss);

  // Deserialize the trie.
  FlexTrie trie1;
  Serializer::Deserialize(trie1, ss);

  // Check the chains are in the trie.
  BOOST_CHECK(TrieUtils::CheckCnainInTrie("hello", 1, trie1));
  BOOST_CHECK(TrieUtils::CheckCnainInTrie("bye", 2, trie1));
}
