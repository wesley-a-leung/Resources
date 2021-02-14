#!/usr/bin/env bash
# first command line argument is the compile command, following arguments are
# the files to compile
numTests=$#
declare -i total=0
declare -i pass=0
declare -i fail=0
for i in $(seq 2 $numTests); do
  test=${!i}
  total+=1
  echo ""
  echo "$test:"
  $1 $test
  retCode=$?
  if (($retCode == 0)); then
    echo "Compiled Successfully"
    pass+=1
  else
    fail+=1
  fi
done

echo ""
echo "$total files(s) compiled"
echo "$pass successful"
echo "$fail failed"

if (($fail != 0)); then
  exit 1
fi
