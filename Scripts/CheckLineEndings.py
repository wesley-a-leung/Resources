# Script to check that every line ends in \n
import sys
input = sys.stdin.readline

exitCode = 0
for i in range(1, len(sys.argv)):
  filename = sys.argv[i]
  with open(filename, "r") as file:
    replacePragmaOnce = False
    curLine = 0
    for line in file:
      curLine += 1
      if line[-1] != '\n':
        print(filename + " contains a line that does not end in \\n")
        exitCode = 1
        break
sys.exit(exitCode)
