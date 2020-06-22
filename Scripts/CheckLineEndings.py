# Script to check that every line ends in \n and not \r\n
import sys
input = sys.stdin.readline

exitCode = 0
for i in range(1, len(sys.argv)):
  filename = sys.argv[i]
  print()
  print(filename + ":")
  with open(filename, "rb") as file:
    replacePragmaOnce = False
    curLine = 0
    for line in file:
      curLine += 1
      if not line.endswith(b"\n"):
        print("Contains a line that does not end in \\n")
        print("First occurrence on line " + str(curLine))
        exitCode = 1
        break
      if line.endswith(b"\r\n"):
        print("Contains a line that end in \\r\\n")
        print("First occurrence on line " + str(curLine))
        exitCode = 1
        break
    else:
      print("Line endings good")
sys.exit(exitCode)
