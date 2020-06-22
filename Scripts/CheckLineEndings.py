# Script to check that every line ends in \n and not \r\n
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
    for line in file:
      curLine += 1
      if not line.endswith(b"\n"):
        print("Contains a line that does not end in \\n")
        print("First occurrence on line " + str(curLine))
        bad += 1
        break
      if line.endswith(b"\r\n"):
        print("Contains a line that end in \\r\\n")
        print("First occurrence on line " + str(curLine))
        bad += 1
        break
    else:
      print("Line endings good")
      good += 1
print()
print(str(total) + " file(s) checked")
print(str(good) + " good")
print(str(bad) + " with errors")
sys.exit(bad != 0)
