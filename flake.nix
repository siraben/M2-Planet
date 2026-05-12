{
  description = "M2-Planet build and AFL++ fuzzing harness";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    M2libc = {
      url = "github:oriansj/M2libc/eee5091e7a1af90b7b87389153647be9a24a8cdd";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, flake-utils, M2libc }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        # M2libc is a submodule; use the pinned input for pure Nix builds.
        m2libcSrc = M2libc;

        mkM2Planet = { cc ? pkgs.gcc, extraEnv ? "" }:
          pkgs.stdenv.mkDerivation {
            pname = "M2-Planet";
            version = "git";
            src = ./.;
            nativeBuildInputs = [ cc pkgs.gnumake ];
            buildPhase = ''
              rm -rf M2libc
              cp -R ${m2libcSrc} M2libc
              chmod -R u+w M2libc
              rm -f bin/M2-Planet
              ${extraEnv}
              CC=${cc}/bin/$(basename $(ls ${cc}/bin/*gcc* 2>/dev/null | head -1) 2>/dev/null || echo gcc) \
                make -j$NIX_BUILD_CORES bin/M2-Planet
            '';
            installPhase = ''
              mkdir -p $out/bin
              cp bin/M2-Planet $out/bin/
            '';
          };

        m2-planet = pkgs.stdenv.mkDerivation {
          pname = "M2-Planet";
          version = "git";
          src = ./.;
          nativeBuildInputs = [ pkgs.gcc pkgs.gnumake ];
          buildPhase = ''
            rm -rf M2libc
            cp -R ${m2libcSrc} M2libc
            chmod -R u+w M2libc
            rm -f bin/M2-Planet
            make -j$NIX_BUILD_CORES bin/M2-Planet
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp bin/M2-Planet $out/bin/
          '';
        };

        m2-planet-afl = pkgs.stdenv.mkDerivation {
          pname = "M2-Planet-afl";
          version = "git";
          src = ./.;
          nativeBuildInputs = [ pkgs.aflplusplus pkgs.gnumake ];
          buildPhase = ''
            rm -rf M2libc
            cp -R ${m2libcSrc} M2libc
            chmod -R u+w M2libc
            rm -f bin/M2-Planet
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
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp bin/M2-Planet $out/bin/M2-Planet
          '';
        };

        fuzz-run = pkgs.writeShellApplication {
          name = "m2-planet-fuzz";
          runtimeInputs = [ pkgs.aflplusplus m2-planet-afl ];
          text = ''
            if [ -z "''${CORPUS:-}" ] && [ -d fuzz/corpus-min ]; then
              CORPUS=fuzz/corpus-min
            fi
            : "''${OUTDIR:=fuzz/findings}"
            : "''${CORPUS:=fuzz/corpus}"
            : "''${DURATION:=600}"
            : "''${TIMEOUT:=100}"
            : "''${DICT:=fuzz/c.dict}"
            : "''${TARGET_OUTPUT:=/dev/null}"
            : "''${ARGS:=--architecture x86}"
            mkdir -p "$OUTDIR"
            ulimit -c 0 || true
            export AFL_SKIP_CPUFREQ=1
            export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1
            read -r -a target_args <<< "$ARGS"
            dict_args=()
            if [ -f "$DICT" ]; then
              dict_args=(-x "$DICT")
            fi
            exec afl-fuzz "''${dict_args[@]}" -i "$CORPUS" -o "$OUTDIR" -t "$TIMEOUT" -m none -V "$DURATION" \
              -- ${m2-planet-afl}/bin/M2-Planet "''${target_args[@]}" -f @@ -o "$TARGET_OUTPUT"
          '';
        };

        fuzz-triage = pkgs.writeShellApplication {
          name = "m2-planet-fuzz-triage";
          runtimeInputs = [ m2-planet pkgs.coreutils ];
          text = ''
            : "''${FINDINGS:=fuzz/findings/default}"
            crashes="$FINDINGS/crashes"
            [ -d "$crashes" ] || { echo "no crashes dir at $crashes"; exit 1; }
            for f in "$crashes"/id:*; do
              [ -f "$f" ] || continue
              echo "=== $(basename "$f") ($(wc -c <"$f") bytes) ==="
              ${m2-planet}/bin/M2-Planet --architecture x86 -f "$f" -o /tmp/triage.M1 2>&1 | head -5
              rc=$?
              echo "rc=$rc"
              echo
            done
          '';
        };

      in {
        packages = {
          inherit m2-planet m2-planet-afl;
          default = m2-planet;
        };

        apps = {
          fuzz = { type = "app"; program = "${fuzz-run}/bin/m2-planet-fuzz"; };
          triage = { type = "app"; program = "${fuzz-triage}/bin/m2-planet-fuzz-triage"; };
        };

        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            aflplusplus
            bash
            coreutils
            findutils
            gcc
            gdb
            git
            gnumake
          ];
        };
      });
}
