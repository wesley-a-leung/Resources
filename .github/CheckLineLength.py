# Script to remove pramga once at the top of C++ header files and replace
# them with header guards
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
      if len(line.strip()) > 79:
        print(filename + " violates line length of 79")
        exitCode = 1
        break
sys.exit(exitCode)
