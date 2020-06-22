# Script to remove pramga once at the top of C++ header files and replace
# them with header guards
import pathlib
import sys
input = sys.stdin.readline

total = 0
replaced = 0
for i in range(1, len(sys.argv)):
  total += 1
  filename = sys.argv[i]
  print()
  print(filename + ":")
  headerguard = pathlib.Path(filename).stem.upper() + "_H"
  output = []
  with open(filename, "r") as file:
    replacePragmaOnce = False
    for line in file:
      if line == "#pragma once\n":
        output.append("#ifndef " + headerguard + "\n")
        output.append("#define " + headerguard + "\n")
        output.append("\n")
        replacePragmaOnce = True
      else:
        output.append(line)
    if replacePragmaOnce:
      output.append("\n")
      output.append("#endif\n")
      print("Replaced #pragma once")
      replaced += 1
    else:
      print("Nothing to replace")
  with open(filename, "w") as file:
    for line in output:
      file.write(line)
print()
print(str(total) + " file(s) checked")
print(str(replaced) + " file(s) modified")
