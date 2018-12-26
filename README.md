# strutext - C++ library of text structuring components

The library implements following components:
* **symbols** - symbolic level processing component;
* **encode** - UNICODE and single-byte encoding implementation.
* **morpho** - morphological analysis implementation for English and Russian languages;
* **automata** - fast finite state machine implementation in memory;
* **utility** - list of utility classes and routins suitable for language processing.

The library is implemented in C++ 2003 for the sake of more compatibility.

## How to use

The shell script called `ubuntu_requirements.sh` will install all required libraries to build the project. In fact, the depends are only Boost and
liblog4cplus libraries.

The directory with name _examples_ contains using examples of the library. Below there is shell script example to build release version of the project.
```sh
git clone git@github.com:merfill/strutext.git
cd strutext
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && make -j20 && make test
```

## symbols

The library implements UNICODE symbol classification routines. There are following types of classes:
* UNICODE inherited classes.
* Upper and Lower subclasses of **Letter** symbol class.

The library is located in subdirectory _symbols_.

### UNICODE symbol classes

Possible classes are:
* **Control**: any symbol, which may be used control text out.
* **Letter**:  letters in any language.
* **Number**:  digits in any language.
* **Separator**: symbol, which may be used as separator in texts (space as example).
* **Punctuator**: the punctuation symbol -- "!,;" and etc.
* **Mark**:       for example, enclosing square, which is used to sign theorem proving.
* **Symbol**:     for example, '$' sign.

The classes may also have sybclasses. See type definition for more information. The SymbolInfo type provides information concerning symbol class
and some extra information, which depends on the symbol class. For instance, for **Letter** class the type contains code of lower and upper
letter variant.

```cpp
typedef uint32_t SymbolCode;

enum SymbolClass {
  UPPERCASE_LETTER      = 0x00000001,
  LOWERCASE_LETTER      = 0x00000002,
  TITLECASE_LETTER      = 0x00000004,
  CASED_LETTER          = UPPERCASE_LETTER | LOWERCASE_LETTER | TITLECASE_LETTER,
  MODIFIER_LETTER       = 0x00000008,
  OTHER_LETTER          = 0x00000010,
  LETTER                = CASED_LETTER | MODIFIER_LETTER | OTHER_LETTER,
  NONSPACING_MARK       = 0x00000020,
  SPACING_MARK          = 0x00000040,
  ENCLOSING_MARK        = 0x00000080,
  MARK                  = NONSPACING_MARK | SPACING_MARK | ENCLOSING_MARK,
  DECIMAL_NUMBER        = 0x00000100,
  LETTER_NUMBER         = 0x00000200,
  OTHER_NUMBER          = 0x00000400,
  NUMBER                = DECIMAL_NUMBER | LETTER_NUMBER | OTHER_NUMBER,
  CONNECTOR_PUNCTUATION = 0x00000800,
  DASH_PUNCTUATION      = 0x00001000,
  OPEN_PUNCTUATION      = 0x00002000,
  CLOSE_PUNCTUATION     = 0x00004000,
  INITIAL_PUNCTUATION   = 0x00008000,
  FINAL_PUNCTUATION     = 0x00010000,
  OTHER_PUNCTUATION     = 0x00020000,
  PUNCTUATION           = CONNECTOR_PUNCTUATION | DASH_PUNCTUATION | OPEN_PUNCTUATION | CLOSE_PUNCTUATION
                          | INITIAL_PUNCTUATION | FINAL_PUNCTUATION | OTHER_PUNCTUATION,
  MATH_SYMBOL           = 0x00040000,
  CURRENCY_SYMBOL       = 0x00080000,
  MODIFIER_SYMBOL       = 0x00100000,
  OTHER_SYMBOL          = 0x00200000,
  SYMBOL                = MATH_SYMBOL | CURRENCY_SYMBOL | MODIFIER_SYMBOL | OTHER_SYMBOL,
  SPACE_SEPARATOR       = 0x00400000,
  LINE_SEPARATOR        = 0x00800000,
  PARAGRAPH_SEPARATOR   = 0x01000000,
  SEPARATOR             = SPACE_SEPARATOR | LINE_SEPARATOR | PARAGRAPH_SEPARATOR,
  CONTROL               = 0x02000000,
  FORMAT                = 0x04000000,
  SURROGATE             = 0x08000000,
  PRIVATE_USE           = 0x10000000,
  UNASSIGNED            = 0x20000000,
  OTHER                 = CONTROL | FORMAT | SURROGATE | PRIVATE_USE | UNASSIGNED
};

// Get class of the symbol
inline const uint32_t& GetSymbolClass(const SymbolCode& code);

// Is symbols in the specified class
template<SymbolClass class_name>
inline bool Is(const SymbolCode& code);

// Definitions for most important classes

inline bool IsCasedLetter(const SymbolCode& code);
inline bool IsLetter(const SymbolCode& code);
inline bool IsMark(const SymbolCode& code);
inline bool IsNumber(const SymbolCode& code);
inline bool IsPunctuation(const SymbolCode& code);
inline bool IsSymbol(const SymbolCode& code);
inline bool IsSeparator(const SymbolCode& code);
inline bool IsOther(const SymbolCode& code);
```

### Upper and lower letters

**Letter** symbol class has two sublclasses that contain lower and upper letter's variants if it's applicable to the letter. The library provides
routines to transform letters to lower and upper representations and to define either the letter is in upper or lower case.

```cpp
bool IsCasedLetter(const SymbolCode& code);
SymbolCode ToLower(const SymbolCode& code);
SymbolCode ToUpper(const SymbolCode& code)
```

### Technical implementation

The class system is based on UNICODE symbol class definition. The UNICODE consortium provides symbol definitions in file called UnicodeData.txt.
This is CSV file where each line contains one symbol definition with class and mapping to related symbols (for instance, upper case for lower case
latter). Below there are several lines from the begining of the file:
```text
0007;<control>;Cc;0;BN;;;;;N;BELL;;;;
0008;<control>;Cc;0;BN;;;;;N;BACKSPACE;;;;
0009;<control>;Cc;0;S;;;;;N;CHARACTER TABULATION;;;;
000A;<control>;Cc;0;B;;;;;N;LINE FEED (LF);;;;
000B;<control>;Cc;0;S;;;;;N;LINE TABULATION;;;;
000C;<control>;Cc;0;WS;;;;;N;FORM FEED (FF);;;;
```

Example of English symbol definitions:
```text
0041;LATIN CAPITAL LETTER A;Lu;0;L;;;;;N;;;;0061;
0042;LATIN CAPITAL LETTER B;Lu;0;L;;;;;N;;;;0062;
0043;LATIN CAPITAL LETTER C;Lu;0;L;;;;;N;;;;0063;
0044;LATIN CAPITAL LETTER D;Lu;0;L;;;;;N;;;;0064;
0045;LATIN CAPITAL LETTER E;Lu;0;L;;;;;N;;;;0065;
0046;LATIN CAPITAL LETTER F;Lu;0;L;;;;;N;;;;0066;
0047;LATIN CAPITAL LETTER G;Lu;0;L;;;;;N;;;;0067;
...
0061;LATIN SMALL LETTER A;Ll;0;L;;;;;N;;;0041;;0041
0062;LATIN SMALL LETTER B;Ll;0;L;;;;;N;;;0042;;0042
0063;LATIN SMALL LETTER C;Ll;0;L;;;;;N;;;0043;;0043
0064;LATIN SMALL LETTER D;Ll;0;L;;;;;N;;;0044;;0044
0065;LATIN SMALL LETTER E;Ll;0;L;;;;;N;;;0045;;0045
0066;LATIN SMALL LETTER F;Ll;0;L;;;;;N;;;0046;;0046
0067;LATIN SMALL LETTER G;Ll;0;L;;;;;N;;;0047;;0047
```

The library implements Python script, which reads UnicodeData.txt during precompilation stage and generates three arrays:
```cpp
extern uint32_t    SYM_CLASS_TABLE[];
extern SymbolCode  SYM_UPPER_TABLE[];
extern SymbolCode  SYM_LOWER_TABLE[];
```
for symbol class, upper and lower letter cases accordingly.

For each symbol code one can define the class in constant time as well as make fast transform to upper or lower cases for cased letters. The arrays
are loaded to the memory when the program runs and this may be the reason for more memory using. However, size of each array is not more 4 MB,
so summary space 12 Mb not seems to be too big payment for the fast implementation approach.

## encode library

Encode library implements rotines for manipulations of symbols in different encodings.

### Utf8Iterator

The library is located in subdirectory _encode_.

The main component of the library is template class Utf8Iterator, which is encoded in `utf8_iterator.h` file. The class is implemented by basing on
boost::iterator\_facade and thus can be used in the code as ordinal C++ STL style iterator. The class allows to extract UNICODE symbols from byte
sequence encoded in UTF-8. The template paremeter of the class is ByteIterator to get bytes from the input sequence.

The extracted symbol is encoded in structure named Utf8Symbol.
```cpp
struct Utf8Symbol {
...
  uint8_t  chain_[6]; // UTF-8 byte sequence read.
  size_t   len_;      // The length of UTF-8 sequence.
  uint32_t utf32_;    // UTF-32 symbol code.
};
```
Here **utf32\_** contains UNICODE code of the extracted symbol, **.GetChain()** references to byte sequence of the extracted symbol and
**GetChainLen()** - length of the extracted byte sequence. Utf8Symbol also implements cast method to `uint32_t` type to get UNICODE code directly
from the iterator.

Below there is the example of extracting UNICODE symbols from byte sequence of Russian text contained in std::string object.
```cpp
typedef strutext::encode::Utf8Iterator<std::string::const_iterator> Utf8Iterator;

std::string input = "Мама мыла раму";
for (Utf8Iterator it = Utf8Iterator(input.begin(), input.end()); it != Utf8Iterator(); ++it) {
  std::cout << *it << ":";
  for (uint32_t id = 0; id < it.GetChainLen(); ++id) {
    std::cout << " " << it.GetChain()[id];
  }
  std::cout << "\n";
}
```

### Utf8Generator

The file `utf8_generator.h` contains routine, which implements generation of UTF-8 sequence from UNICODE symbol code. The routine code is following:
```cpp
template <typename ByteIterator>
inline ByteIterator GetUtf8Sequence(strutext::symbols::SymbolCode code, ByteIterator oi);

template <typename Utf32Iterator, typename ByteIterator>
inline ByteIterator GetUtf8Sequence(Utf32Iterator begin, Utf32Iterator end, ByteIterator oi);
```

The function gets symbol code on the input and produced UTF-8 sequence to the passed output byte iterator. The second version of the routine gets
a sequence of UNICODE symbols as the input. Below there is the code example:
```cpp
std::string result;
strutext::encode::GetUtf8Sequence(0x41, std::back_inserter(result));
```

### Single byte encodings

The library also impements variety of single byte to UNICODE encoder iterators for Russian and Ukraine languages. Below there is the definition
of these encoders:
```cpp
#include "char_iterator.h"
#include "char_unicode32_decoder.h"

typedef strutext::encode::CharIterator<const char*, strutext::encode::Cp1251Decoder>      Cp1251Iterator;
typedef strutext::encode::CharIterator<const char*, strutext::encode::Cp1252Decoder>      Cp1252Iterator;
typedef strutext::encode::CharIterator<const char*, strutext::encode::Cp1253Decoder>      Cp1253Iterator;
typedef strutext::encode::CharIterator<const char*, strutext::encode::Cp866Decoder>       Cp866Iterator;
typedef strutext::encode::CharIterator<const char*, strutext::encode::Iso88591Decoder>    Iso88591Iterator;
typedef strutext::encode::CharIterator<const char*, strutext::encode::Koi8ruDecoder>      Koi8ruIterator;
typedef strutext::encode::CharIterator<const char*, strutext::encode::Koi8uDecoder>       Koi8uIterator;
typedef strutext::encode::CharIterator<const char*, strutext::encode::Koi8rDecoder>       Koi8r1Iterator;
typedef strutext::encode::CharIterator<const char*, strutext::encode::MacCyrillicDecoder> MacCyrillicIterator;
typedef strutext::encode::CharIterator<const char*, strutext::encode::MacUkraineDecoder>  MacUkraineIterator;

std::string word = "some Russian text in cp1251";
for (Cp1251Iterator it(word.begin(), word.end(), end; it != end; ++it) {
  std::cout << *it << "\n";
}
```

## morpho library

### Morphologist

The library implements morphological analysis algorithms for English and Russia languages. The main library interface is defined in
`morpho/morpholib/morpho.h` file. the file contains definitions of classes `MorphologistBase`, which defines interface to the library and inherited
`Morphologist` template class, which is parametrized by Alphabet class. It's needed to define following types to use library:
```cpp
#include "rus_alphabet.h"
#include "eng_alphabet.h"
#include "morpho_modifier.h"
#include "morpho.h"
#include "rus_model.h"
#include "eng_model.h"

typedef strutext::morpho::Morphologist<strutext::morpho::EnglishAlphabet> EnglishMorpher;
typedef strutext::morpho::Morphologist<strutext::morpho::RussianAlphabet> RussianMorpher;
```
Here `EnglishAlphabet` and `RussianAlphabet` are classes defined in `morpho/alphabets` directory. Each alphabet symbol in morpho library is
implemented in one byte encoding schema for optimization purposes.`Alphabet` class implementation implements decoding/encoding routines to/from
UNICODE symbols.

The `Morphologist` class object contains list of all word forms in the dictionary (Russian or English). An each word (lemma) in the dictionary has
list of forms. For instance, lemma _say_ has two forms: _say_ ans _says_. The first form _say_ is called main form of the lemma. So, lemma can be
defined just as a list of its forms, presented by its main form. An each form in the list lexical attributes. We'll discuss lexical attributes
later, in the section _Language Models_.

The `Morphologist` class provides method `Analize`, which gets form text in UTF-8 encoding and returns list of possible lemmas for the given form.
One form can be in more than one lemma. For example, form _say_ can be in two lemmas: noun _say_ and adverb _say_. The method definition is:
```cpp
void Analize(const std::string& text, LemList& lem_list) const;
```

Here the definition of `LemList`:
```cpp
struct Lemma {
...
  uint32_t id_;   ///< Lemma identifier.
  uint32_t attr_; ///< Form attributes.
};

/// Lemma list type definition.
typedef std::list<Lemma> LemList;
```

Thus, `Lemma` contains unique identifier and list of lexical attributes. This list is encoded in 4 bytes only and `Morphologist` class provides
special methods to generate form UTF-8 text from encoded attribute list and lemma id: `Generate` ans `GenAllForms`:
```cpp
std::string Generate(uint32_t lem_id, uint32_t attrs) const;
size_t GenAllForms(uint32_t lem_id, std::set<std::string>& form_set) const;
```

One can also generate main form for passed lemma identifier:
```cpp
bool GenMainForm(uint32_t lem_id, std::string& main_form) const;
```

The `Morphologist` class is also serializable and must be initialized by dictionary comming from std::stream:
```cpp
void Serialize(std::ostream& os) const;
void Deserialize(std::istream& is);
```

The serialized dictionary within lexical information should be generated by special utility from text representation. We'll discuss below how to do
this as well as how to operate extracted lexical attributes for specified language model.

### Language Models

Language model is definition of lexical attributes that can be assigned to forms. The base class `PartOfSpeech` allows to operate models for different
languages by using the same abstract type.

For the moment, two langauge models are implemented: for Russian and English languages. The part of speech classes are inherited from `EnglishPos`
and `RussianPos` respectively. The concrete POS classes are, for example: Noun, Adjective, Verb, and etc. An each POS class is serializable to
`uint32_t` type. The `PosSerializer` class provides interface to extract POS class instance from `uint32_t` object.
```cpp
static EnglishPos::Ptr Deserialize(const uint32_t& ob);
static RussianPos::Ptr Deserialize(const uint32_t& ob);
```
The library also provide human understandable description of POS for English and Russian languages. This description is located to 
`morpho/models/lang_model_description.h`, where `lang` can be either `rus` or `eng`.

The using example will be provided below. Before we need to understand the structure of dictionaries and how to generate binary
representation from the text one.

### Dictionaries

The dictionaries (English and Russian) are got from http://aot.ru. Each dictionary has implemented in two files: `lang_tabs.txt` and `lang_morphs.txt`,
where _lang_ is either eng or rus.

Tabs file contains lexical attribute definitions that are encode by two letters sequence. For example:
```
aa 1 ADJECTIVE
ab 1 ADJECTIVE comp
ac 1 ADJECTIVE sup
...
na 1 NOUN narr,sg
nb 1 NOUN narr,pl
```

File morphs consists of several parts. By historical reasons only two sections are used:
1. Line section.
2. Morph section.

#### Line section

This is the sequence of lines. An each line contains a set of suffixes along with lexical attribute definition. An each suffix is concatenated to
prefix defined in Morph section to produce the form with assigned lexical attributes. For instance:
```
%*va%S*vb%ED*vc%ED*vd%ING*ve
```
can be used to generate lemma `ABANDON` with forms: `ABANDON` (VERB inf), `ABANDONS` (VERB prsa,sg,3), `ABANDONED` (VERB pasa),
`ABANDONING` (VERB ing).

#### Morph section

The section contains lemma definitions. Lemma is defined by its prefix text and number of line in Line section. For example:
```
ABALONE 6 0 0 - -
ABAMPERE 6 0 0 - -
ABANDON 6 0 0 - -
ABANDON 8 2 0 - -
ABANDONED 5 1 0 - -
ABANDONEE 6 0 0 - -
```
Here first field is prefix text and the second field is number of line in Line section (numbered starting with zero).

#### Dictionary genertion

All this information is encoded in dictionary implementation in morpho library. Utility called `aot-parser` is used to generate binary representation
of English and Russian dictionaries. This representation can be then deserialized in `Morphologist` class object.

```
aot-parser --help
Allowed options:
  --help                produce help
  -t [ --tab ] arg      tab file name
  -d [ --dict ] arg     dictionary file name
  -b [ --bin ] arg      binary dictionary file name
  -m [ --model ] arg    language model: eng, rus
  -v [ --verbose ]      produce process info to stderr
```

```
aot-parser -t morpho/aot/eng_tabs.txt -d morpho/aot/eng_morphs.txt -b eng_dict.bin -m eng
aot-parser -t morpho/aot/rus_tabs.txt -d morpho/aot/rus_morphs.txt -b rus_dict.bin -m rus
```

These two commands generate English and Russian binary representations of dictionaries.

### How to use

Here the the example of main form generation for Russian dictionary.

```cpp
typedef strutext::morpho::Morphologist<strutext::morpho::RussianAlphabet> Morpher;

std::ifstream dict("rus_dict.bin");
if (not dict.is_open()) {
  throw std::invalid_argument("Cannot open russian dictionary: rus_dict.bin");
}
Morpher morpher;
morpher.Deserialize(dict);

std::string text = "мыла";
strutext::morpho::MorphologistBase::LemList lemmas;
morpher.>Analize(text, lemmas);

// Extract main forms.
std::set<std::string> forms;
for (const auto& lemma : lemmas) {
  std::string main_form;
  if (morpher.GenMainForm(lemma.id_, main_form)) {
    std::cout << main_form << "\n";
  }
}
```

## automata library


## utility library



