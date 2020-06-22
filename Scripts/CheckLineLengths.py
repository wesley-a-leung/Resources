# Script to check that the length of a line does not exceed 79 characters
# Commented out for now
import sys
input = sys.stdin.readline

total = 0
good = 0
bad = 0
# for i in range(1, len(sys.argv)):
#   filename = sys.argv[i]
#   print()
#   print(filename + ":")
#   with open(filename, "rb") as file:
#     replacePragmaOnce = False
#     curLine = 0
#     for line in file:
#       curLine += 1
#       if line.endswith(b"\n"):
#         line = line[:-1]
#       if len(line) > 79:
#         print("Contains a line violating maximum line length of 79")
#         print("First occurrence on line " + str(curLine))
#         bad += 1
#         break
#     else:
#       print("Line lengths good")
#       good += 1
print()
print(str(total) + " file(s) checked")
print(str(good) + " good")
print(str(bad) + " with errors")
sys.exit(bad != 0)
