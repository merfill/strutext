# Copyright &copy; 2021, Vladimir Lapshin.
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

import argparse
import sys
import os

# Generate prolog in binary symbol table.
def generateFileProlog(f):
  f.write("/** Copyright &copy; 2021, Vladimir Lapshin.\n")
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
  f.write(" * \\brief Automatically generated Unihan hierogliph table.\n")
  f.write(" * \\author Vladimir Lapshin.\n")
  f.write(" */\n\n")
  f.write("#include \"symbols.h\"\n\n")
  f.write("namespace strutext { namespace symbols { namespace details {\n\n")

def generateTableProlog(f, t, n):
    f.write("{0} {1}[] = {{\n".format(t,n))

def generateFileEpilog(f):
  f.write("}}} // namespace strutext, symbols, details.\n")

def generateTableEpilog(f, t, n):
    f.write("}}; // {0} {1}\n\n".format(t,n))

def generateData(input_path, output_path):
    with open(input_path, 'r') as inf, open(output_path, 'w') as outf:
        max_code = 0
        codes = dict()
        for line in inf:
            line = line.rstrip()
            if line.startswith('#') or len(line) == 0:
                continue
            code, _, _ = line.split('\t')
            code = int(code[2:], 16)
            if code > max_code:
                max_code = code
                codes[code] = True
        generateFileProlog(outf)
        outf.write('bool UNIHAN_TABLE[] = {\n')
        for code in range(max_code):
            if code in codes:
                outf.write('  true,\n')
            else:
                outf.write('  false,\n')
        outf.write('  true\n')
        outf.write('};\n\n')
        outf.write('unsigned UNIHAN_TABLE_SIZE = {};\n\n'.format(max_code+1))
        outf.write("}}} // namespace strutext, symbols, details.\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", help="path to input Unihan definition file")
    parser.add_argument("-o", "--output", help="path to output C++ file")
    args = parser.parse_args()

    if not os.path.exists(args.input):
        raise RuntimeError("{} does not exist".format(args.input))
    else:
        generateData(args.input, args.output)

