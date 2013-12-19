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
 * \brief  Finite State Machine base types and constants.
 * \author Vladimir Lapshin.
 */

#pragma once

#include <stdint.h>

namespace strutext { namespace automata {

/// Type of state index.
typedef uint32_t StateId;

/// Index of start state..
static const StateId kStartState = 1;

/// Index of incorrect state.
static const StateId kInvalidState = 0;

}} // namespace strutext, automata.
