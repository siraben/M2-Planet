int inc(int x) { return x + 8; }
int dec(int x) { return x - 8; }
int mul(int x) { return x * 8; }
int bits(int x) { return (x & 255) | 1024; }
int xors(int x) { return x ^ 255; }
int constants(int x) { return x + 2 * 8 + (6 | 1) - (32 - 4); }
int main() {
 int x; unsigned u;
 for(x=0-200; x<200; x=x+1) {
  if (inc(x) - x != 8) return 1;
  if (dec(x) - x != -8) return 2;
  if (mul(x) / 8 != x) return 3;
  if (bits(x) < 1024) return 4;
  if (bits(x) >= 1280) return 5;
  if ((xors(x) ^ x) != 255) return 6;
  if (constants(x) - x != -5) return 7;
 }
 u=0-1;
 if (u+8 != 7) return 8;
 if (u*8 != u-7) return 9;
 if (sizeof(int) > 4) {
 u=1; u=u<<40;
 if (u+8-u != 8) return 10;
 if (u-8+8 != u) return 11;
 }
 return 0;
}
