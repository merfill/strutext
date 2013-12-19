# Copyright &copy; 2013, Vladimir Lapshin.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
# \author Vladimir Lapshin
# \date   08.10.2013

import getopt, sys

# Generate prolog in binary symbol table.
def generateFileProlog(f):
  f.write("/** Copyright &copy; 2013, Vladimir Lapshin.\n")
  f.write(" *\n");
  f.write(" *   Licensed under the Apache License, Version 2.0 (the \"License\");\n");
  f.write(" *   you may not use this file except in compliance with the License.\n");
  f.write(" *   You may obtain a copy of the License at\n");
  f.write(" *\n");
  f.write(" *     http://www.apache.org/licenses/LICENSE-2.0\n");
  f.write(" *\n");
  f.write(" *   Unless required by applicable law or agreed to in writing, software\n");
  f.write(" *   distributed under the License is distributed on an \"AS IS\" BASIS,\n");
  f.write(" *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n");
  f.write(" *   See the License for the specific language governing permissions and\n");
  f.write(" *   limitations under the License.\n");
  f.write(" *\n");
  f.write(" * \\brief Automatically generated UNICODE symbol table.\n")
  f.write(" * \\author Vladimir Lapshin.\n")
  f.write(" */\n\n")
  f.write("#include \"symbols.h\"\n\n")
  f.write("namespace strutext { namespace symbols { namespace details {\n\n")

def generateTableProlog(f, t, n):
    f.write("{0} {1}[] = {{\n".format(t,n))

# Generate epilog.
def generateFileEpilog(f):
  f.write("}}} // namespace strutext, symbols, details.\n")

def generateTableEpilog(f, t, n):
    f.write("}}; // {0} {1}\n\n".format(t,n))

def generateClassMapping(m):
  m["Lu"] = "UPPERCASE_LETTER"
  m["Ll"] = "LOWERCASE_LETTER"
  m["Lt"] = "TITLECASE_LETTER"
  m["Lm"] = "MODIFIER_LETTER"
  m["Lo"] = "OTHER_LETTER"
  m["Mn"] = "NONSPACING_MARK"
  m["Mc"] = "SPACING_MARK"
  m["Me"] = "ENCLOSING_MARK"
  m["Nd"] = "DECIMAL_NUMBER"
  m["Nl"] = "LETTER_NUMBER"
  m["No"] = "OTHER_NUMBER"
  m["Pc"] = "CONNECTOR_PUNCTUATION"
  m["Pd"] = "DASH_PUNCTUATION"
  m["Ps"] = "OPEN_PUNCTUATION"
  m["Pe"] = "CLOSE_PUNCTUATION"
  m["Pi"] = "INITIAL_PUNCTUATION"
  m["Pf"] = "FINAL_PUNCTUATION"
  m["Po"] = "OTHER_PUNCTUATION"
  m["Sm"] = "MATH_SYMBOL"
  m["Sc"] = "CURRENCY_SYMBOL"
  m["Sk"] = "MODIFIER_SYMBOL"
  m["So"] = "OTHER_SYMBOL"
  m["Zs"] = "SPACE_SEPARATOR"
  m["Zl"] = "LINE_SEPARATOR"
  m["Zp"] = "PARAGRAPH_SEPARATOR"
  m["Cc"] = "CONTROL"
  m["Cf"] = "FORMAT"
  m["Cs"] = "SURROGATE"
  m["Co"] = "PRIVATE_USE"
  m["Cn"] = "UNASSIGNED"

def generate(textTablePath, binTablePath):
  symbols = {}
  classes = {}
  maxCode = 0
  symbol = {}
  try:
    # Open UNICODE data file and read symbol informations from it.
    f = open(textTablePath, 'rb')
    for line in f:
      lineParams = line.split(';')
      if lineParams.__len__() <> 15:
        print "Incorrect UNICODE data file format: '", line, "'"
        sys.exit(4)
      else:
        # Remember symbol properties.
        symCode = int(lineParams[0], 16)
        symbols[symCode] = lineParams
        if maxCode < symCode:
          maxCode = symCode
    f.close()

    # Trying to generate output.
    f = open(binTablePath, 'wb')
    generateFileProlog(f)

    # Generate symbol classes from 0 to 2^21 - 1.
    generateClassMapping(classes)
    generateTableProlog(f, "uint32_t", "SYM_CLASS_TABLE")
    for code in range(0, maxCode + 1):
      f.write("  ")
      if code > 0:
        f.write(",")
      if code in symbols:
        symbol = symbols[code]
        if symbol[2] in classes:
          f.write("static_cast<uint32_t>({0}) // {1}.\n".format(classes[symbol[2]], symbol[1]))
        else:
            print "Incorrect symbol class {0} for symbol {1}".format(symbol[2], symbol[0])
            sys.exit(5)
      else:
        f.write("static_cast<uint32_t>(UNASSIGNED)\n")
    generateTableEpilog(f, "uint32_t", "SYM_CLASS_TABLE")

    # Generate symbol upper counterparts from 0 to 2^21 - 1.
    generateTableProlog(f, "SymbolCode", "SYM_UPPER_TABLE")
    for code in range(0, maxCode + 1):
        f.write("  ")
        if code > 0:
            f.write(",")
        if code in symbols:
            symbol = symbols[code]
            f.write("0x{0} // {1}.\n".format(symbol[12] if symbol[12].__len__() > 0 else symbol[0], symbol[1]))
        else:
            f.write("0x{0:04X}\n".format(code))
    generateTableEpilog(f, "SymbolCode", "SYM_UPPER_TABLE")

    # Generate symbol upper counterparts from 0 to 2^21 - 1.
    generateTableProlog(f, "SymbolCode", "SYM_LOWER_TABLE")
    for code in range(0, maxCode + 1):
        f.write("  ")
        if code > 0:
            f.write(",")
        if code in symbols:
            symbol = symbols[code]
            f.write("0x{0} // {1}.\n".format(symbol[13] if symbol[13].__len__() > 0 else symbol[0], symbol[1]))
        else:
            f.write("0x{0:04X}\n".format(code))
    generateTableEpilog(f, "SymbolCode", "SYM_LOWER_TABLE")

    generateFileEpilog(f)
    f.close()
  except IOError as (strerror):
    print "Error: {0}".format(strerror)
    sys.exit(3)

def usage():
  print "usage:\n  [--help]\n  --from='UNICODE text file'\n  --to='symbol table file'"

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:], "hf:t:", ["help", "from=", "to="])
  except getopt.GetoptError, err:
    print str(err)
    usage()
    sys.exit(2)

  textTablePath = None
  binTablePath = None
  for o, a in opts:
    if o in ("-f", "--from"):
      # from -- the UNICODE data file path.
      textTablePath = a
    elif o in ("-t", "--to"):
      # to -- the generated table file path.
      binTablePath = a
    elif o in ("-h", "--help"):
      usage()
      sys.exit()
    else:
      assert False, "unhandled option"

  # Read input file and generate symbol table.
  generate(textTablePath, binTablePath)

if __name__ == "__main__":
  main()

