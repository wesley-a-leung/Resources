#!/usr/bin/env bash
ulimit -s 524288
numTests=$#
declare -i total=0
declare -i pass=0
declare -i fail=0
for i in $(seq 1 $numTests); do
  test=${!i}
  total+=1
  echo ""
  echo "$(basename $test):"
  g++ -std=c++17 -O2 -Wall -Wextra -pedantic-errors -Werror $test -o Test.o
  retCode=$?
  if (($retCode == 0)); then
    timeout 1m ./Test.o
    retCode=$?
    if (($retCode == 0)); then
      pass+=1
    else
      fail+=1
      echo "Test Failed"
      echo "  Exit Code: $retCode"
    fi
    rm Test.o
  else
    fail+=1
    echo "Failed to compile"
  fi
done

echo ""
echo "$total test(s) ran"
echo "$pass passed"
echo "$fail failed"

if (($fail != 0)); then
  exit 1
fi

