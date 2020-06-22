# Script to check that every line ends in \n and not \r\n, and that every line
# does not exceed 79 characters
import sys
input = sys.stdin.readline

total = 0
good = 0
bad = 0
for i in range(1, len(sys.argv)):
  filename = sys.argv[i]
  print()
  print(filename + ":")
  with open(filename, "rb") as file:
    total += 1
    replacePragmaOnce = False
    curLine = 0
    ok = True
    for line in file:
      curLine += 1
      if not line.endswith(b"\n"):
        print("line " + str(curLine) + " does not end in \\n")
        ok = False
      if line.endswith(b"\r\n"):
        print("line " + str(curLine) + " ends in \\r\\n")
        ok = False
      # if len(line.rstrip(b"\r\n")) > 79:
      #   print("line " + str(curLine) + " violates maximum line length of 79")
      #   ok = False
    if ok:
      print("All lines good")
      good += 1
    else:
      bad += 1
print()
print(str(total) + " file(s) checked")
print(str(good) + " good")
print(str(bad) + " with errors")
sys.exit(bad != 0)
