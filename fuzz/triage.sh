#!/usr/bin/env bash
# Re-runs each fuzzer-found crash against an uninstrumented M2-Planet
# and classifies the failure by signal + the last function name in the
# stderr stack frame (when available).
#
# Usage: bash fuzz/triage.sh [findings/default]

set -u
FINDINGS=${1:-fuzz/findings/default}
CRASHES=$FINDINGS/crashes
HANGS=$FINDINGS/hangs

if [ ! -x ./bin/M2-Planet ]; then
    echo "build ./bin/M2-Planet first (un-instrumented, plain gcc)"
    exit 1
fi

classify() {
    local f=$1
    local out
    out=$(./bin/M2-Planet --architecture x86 -f "$f" -o /tmp/triage.M1 2>&1)
    local rc=$?
    if [ $rc -eq 139 ]; then
        # Try to extract a location hint from the binary's own error printing
        # (M2-Planet has line_error / require which print file:line). If empty
        # the crash is silent; fall back to "SIGSEGV (silent)".
        local hint
        hint=$(printf '%s\n' "$out" | grep -Eo '[a-zA-Z_]+\.c:[0-9]+' | head -1)
        if [ -n "$hint" ]; then
            echo "SIGSEGV at $hint"
        else
            echo "SIGSEGV (silent)"
        fi
    elif [ $rc -eq 134 ]; then
        echo "SIGABRT (assertion)"
    elif [ $rc -eq 0 ]; then
        echo "exit 0 (false positive, no longer crashes)"
    else
        echo "exit $rc"
    fi
}

echo "=== CRASHES ==="
for f in "$CRASHES"/id:*; do
    [ -f "$f" ] || continue
    printf '%-60s %s\n' "$(basename "$f")" "$(classify "$f")"
done

echo
echo "=== HANGS (>5s, sample first 5) ==="
n=0
for f in "$HANGS"/id:*; do
    [ -f "$f" ] || continue
    n=$((n+1))
    [ $n -gt 5 ] && break
    printf '%-60s %s\n' "$(basename "$f")" "(size $(wc -c <"$f"))"
done
