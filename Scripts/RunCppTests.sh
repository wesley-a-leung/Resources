#!/usr/bin/env bash
# first command line argument is the compile command, following arguments are
# the files to compile
ulimit -s 524288
numArgs=$#
declare -i total=0
declare -i pass=0
declare -i fail=0
for i in $(seq 2 $numArgs); do
  test=${!i}
  total+=1
  echo ""
  echo "$test:"
  $1 $test -o $test.o
  retCode=$?
  if (($retCode == 0)); then
    timeout 5m ./$test.o
    retCode=$?
    if (($retCode == 0)); then
      pass+=1
    else
      fail+=1
      echo "Test Failed"
      echo "  Exit Code: $retCode"
    fi
    rm $test.o
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
