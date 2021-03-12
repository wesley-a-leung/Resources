import argparse
import sys

parser = argparse.ArgumentParser(
  description="Script to check that every line ends in \\n and not \\r\\n, "
              "does not contain \\t, "
              "and that every line does not exceed 79 characters",
)
parser.add_argument("filenames", metavar="file", type=str, nargs="+")
filenames = parser.parse_args().filenames

good = 0
bad = 0
for filename in filenames:
  print()
  print(filename + ":")
  with open(filename, "rb") as file:
    ok = True
    for curLine, line in enumerate(file, 1):
      if not line.endswith(b"\n"):
        print(f"line {curLine} does not end in \\n")
        ok = False
      if line.endswith(b"\r\n"):
        print(f"line {curLine} ends in \\r\\n")
        ok = False
      if line.find(b"\t") != -1:
        print(f"line {curLine} contains \\t")
        ok = False
      if line.find(b"http") != -1 and len(line.rstrip(b"\r\n")) > 79:
        print(f"line {curLine} exceeds maximum line length of 79")
        ok = False
    if ok:
      print("All lines good")
      good += 1
    else:
      bad += 1

print()
print(len(filenames), "file(s) checked")
print(good, "good")
print(bad, "with errors")
sys.exit(bad != 0)
