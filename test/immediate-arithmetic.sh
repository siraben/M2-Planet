#!/usr/bin/env sh
# Require both immediate selection and bounded constant folding to be exercised.
set -eu
mkdir -p test/run-pass/tmp/amd64
output=test/run-pass/tmp/amd64/immediate-arithmetic-codegen.M1
bin/M2-Planet --architecture amd64 --expand-includes \
  test/run-pass/immediate_arithmetic.c -o "$output"
grep -q 'immediate RHS' "$output"
grep -q 'folded literal operands' "$output"
# Constant-RHS comparisons should also reach the direct branch path.
awk '/483D.*immediate RHS/ {
  if (getline > 0 && $0 ~ /0F8/) found = 1
}
END { exit !found }' "$output"
