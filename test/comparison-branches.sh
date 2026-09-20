#!/usr/bin/env sh
# Verify the optimization is exercised, as well as its run-pass semantics.
set -eu
mkdir -p test/run-pass/tmp/amd64
output=test/run-pass/tmp/amd64/comparison-branches-codegen.M1
bin/M2-Planet --architecture amd64 --expand-includes \
  test/run-pass/comparison_branches.c -o "$output"
# Signed/unsigned ordering and equality, with both branch polarities.
for condition in 82 83 84 85 86 87 8C 8D 8E 8F; do
  grep -q "'0F$condition' %" "$output"
done
# Comparisons whose value is stored or used in arithmetic still materialize it.
grep -q 'movzx_rax,al' "$output"
