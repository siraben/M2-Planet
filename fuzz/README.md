# Fuzzing M2-Planet with AFL++

This directory holds a small AFL++ fuzzing harness for M2-Planet.

## Quickstart

```sh
# Build instrumented compiler + run a 10-minute fuzz campaign:
nix run .#fuzz

# Re-run individual crashes against the *unstrumented* compiler to
# distinguish real bugs from AFL artefacts:
nix run .#triage
```

Or by hand:

```sh
nix develop
make clean
mkdir -p bin
afl-gcc-fast -D_GNU_SOURCE -O2 -std=c99 -g -Wall -Wextra -Wstrict-prototypes \
  M2libc/bootstrappable.c \
  cc_reader.c \
  cc_strings.c \
  cc_types.c \
  cc_emit.c \
  cc_core.c \
  cc_macro.c \
  cc.c \
  cc_globals.c \
  -o bin/M2-Planet
mkdir -p fuzz/findings
AFL_SKIP_CPUFREQ=1 \
AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1 \
afl-fuzz -x fuzz/c.dict -i fuzz/corpus -o fuzz/findings -t 100 -m none -V 600 \
  -- ./bin/M2-Planet --architecture x86 -f @@ -o /dev/null
```

The Nix app accepts these environment variables:

- `ARGS` target arguments before `-f @@`, default `--architecture x86`.
- `CORPUS` input corpus directory, default `fuzz/corpus-min` when present,
  otherwise `fuzz/corpus`.
- `DICT` AFL dictionary, default `fuzz/c.dict`.
- `DURATION` run length in seconds, default `600`.
- `OUTDIR` AFL findings directory, default `fuzz/findings`.
- `TARGET_OUTPUT` compiler output path, default `/dev/null`.
- `TIMEOUT` AFL timeout in milliseconds, default `100`.

Useful campaigns for broader coverage:

```sh
ARGS="--architecture amd64" nix run .#fuzz
ARGS="--architecture riscv32" nix run .#fuzz
ARGS="--architecture riscv64" nix run .#fuzz
ARGS="--architecture armv7l" nix run .#fuzz
ARGS="--architecture aarch64" nix run .#fuzz
ARGS="--architecture knight-posix" nix run .#fuzz
ARGS="--architecture x86 -E --expand-includes -I M2libc -I test/run-pass" nix run .#fuzz
ARGS="--architecture x86 --bootstrap-mode" nix run .#fuzz
ARGS="--architecture x86 --max-string 8" nix run .#fuzz
```

## Corpus

`fuzz/corpus/` contains the seed inputs. Refresh it with:

```sh
./fuzz/refresh-corpus.sh
```

- Every `test/run-pass/*.c`/`.h` and `test/compile-fail/*.c` file under 4 KiB.
- Four hand-written minimal C programs (`zero.c`, `expr.c`, `arr.c`,
  `fp.c`) to encourage variety in early mutations.
- A few tiny malformed inputs (`a`, `int`, `{`, `#`, unterminated
  string/comment) to exercise lexer and error paths.

To minimize the corpus after refreshing:

```sh
afl-cmin -i fuzz/corpus -o fuzz/corpus-min -- \
  ./bin/M2-Planet --architecture x86 -f @@ -o /dev/null
```

The Nix runner automatically uses `fuzz/corpus-min` when it exists.
Fuzzing outputs are written under `fuzz/findings/` by default and are
ignored by Git.
