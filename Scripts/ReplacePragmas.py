import argparse
import pathlib

parser = argparse.ArgumentParser(
  description="Script to remove pramga once at the top of C++ header files "
              "and replace them with header guards",
)
parser.add_argument("filenames", metavar="file", type=str, nargs="+")
filenames = parser.parse_args().filenames

replaced = 0
for filename in filenames:
  print()
  print(filename + ":")
  headerguard = pathlib.Path(filename).stem.upper() + "_H"
  output = []
  with open(filename, "r") as file:
    replacePragmaOnce = False
    for line in file.read().split("\n"):
      if line == "#pragma once":
        output.append("#ifndef " + headerguard)
        output.append("#define " + headerguard)
        output.append("")
        replacePragmaOnce = True
      else:
        output.append(line)
    if replacePragmaOnce:
      output.append("")
      output.append("#endif")
      print("Replaced #pragma once")
      replaced += 1
    else:
      print("Nothing to replace")
  with open(filename, "w") as file:
    file.write("\n".join(output) + "\n")

print()
print(len(filenames), "file(s) checked")
print(replaced, "file(s) modified")
