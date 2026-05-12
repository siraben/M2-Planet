#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."

mkdir -p fuzz/corpus

find test/run-pass test/compile-fail \
  -type f \( -name '*.c' -o -name '*.h' \) \
  -size -4k \
  -exec cp {} fuzz/corpus/ \;

cat > fuzz/corpus/zero.c <<'EOF'
int main() { return 0; }
EOF

cat > fuzz/corpus/expr.c <<'EOF'
int main() { int x = 1+2; return x; }
EOF

cat > fuzz/corpus/arr.c <<'EOF'
int a[3] = {1,2,3}; int main() { return a[0]; }
EOF

cat > fuzz/corpus/fp.c <<'EOF'
typedef int (*F)(int); int id(int x){return x;} int main(){F f=id; return f(1);}
EOF

cat > fuzz/corpus/single_identifier.c <<'EOF'
a
EOF

cat > fuzz/corpus/keyword_only.c <<'EOF'
int
EOF

cat > fuzz/corpus/open_brace.c <<'EOF'
{
EOF

cat > fuzz/corpus/preprocessor_marker.c <<'EOF'
#
EOF

cat > fuzz/corpus/unterminated_string.c <<'EOF'
"
EOF

cat > fuzz/corpus/unterminated_comment.c <<'EOF'
/*
EOF
