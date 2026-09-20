/* Preserve M2 eager &&/|| evaluation, unlike ISO C. */
int calls;
int tick(int n) { calls = calls + 1; return n; }
int signed_bits(int a, int b) {
  int r; r = 0;
  if (a < b) r = r + 1;
  if (a <= b) r = r + 2;
  if (a > b) r = r + 4;
  if (a >= b) r = r + 8;
  if (a == b) r = r + 16;
  if (a != b) r = r + 32;
  return r;
}
int unsigned_bits(unsigned a, unsigned b) {
  int r; r = 0;
  if (a < b) r = r + 1;
  if (a <= b) r = r + 2;
  if (a > b) r = r + 4;
  if (a >= b) r = r + 8;
  if (a == b) r = r + 16;
  if (a != b) r = r + 32;
  return r;
}
int main() {
  int a; int b; int r; unsigned u;
  if (signed_bits(-7, 3) != 35) return 1;
  if (signed_bits(3, -7) != 44) return 2;
  if (signed_bits(-7, -7) != 26) return 3;
  if (signed_bits(0, 0) != 26) return 4;
  u = 0 - 1;
  if (unsigned_bits(u, 0) != 44) return 5;
  if (unsigned_bits(0, u) != 35) return 6;
  if (unsigned_bits(u, u) != 26) return 7;
  a = -3; r = 0;
  do { r = r + 1; a = a + 1; } while (a < 3);
  if (r != 6) return 8;
  a = -3; r = 0;
  do { r = r + 1; a = a + 1; } while (a <= 3);
  if (r != 7) return 9;
  a = 3; r = 0;
  do { r = r + 1; a = a - 1; } while (a > -3);
  if (r != 6) return 10;
  a = 3; r = 0;
  do { r = r + 1; a = a - 1; } while (a >= -3);
  if (r != 7) return 11;
  a = 3; r = 0;
  do { r = r + 1; a = a - 1; } while (a != 0);
  if (r != 3) return 12;
  a = 0; r = 0;
  do { r = r + 1; a = a + 1; } while (a == 1);
  if (r != 2) return 13;
  u = 0; r = 0;
  do { r = r + 1; u = u + 1; } while (u < 3);
  if (r != 3) return 14;
  u = 0; r = 0;
  do { r = r + 1; u = u + 1; } while (u <= 3);
  if (r != 4) return 15;
  u = 3; r = 0;
  do { r = r + 1; u = u - 1; } while (u > 0);
  if (r != 3) return 16;
  u = 3; r = 0;
  do { r = r + 1; u = u - 1; } while (u >= 1);
  if (r != 3) return 17;
  a = 1; b = 2;
  if (!(a < b)) return 18;
  if (!(a != b)) return 19;
  if (!(!(a == b))) return 20;
  if (r = (a < b)) { if (r != 1) return 21; }
  else return 22;
  r = (a < b) + (a == b) + (a != b);
  if (r != 2) return 23;
  calls = 0;
  if (tick(0) && tick(1)) return 24;
  if (calls != 2) return 25;
  calls = 0;
  if (tick(0) || tick(1)) { if (calls != 2) return 26; }
  else return 27;
  a = -2; r = 0;
  while (a < 2) { r = r + 1; a = a + 1; }
  if (r != 4) return 28;
  r = 0;
  for (a = -2; a < 2; a = a + 1) r = r + 1;
  if (r != 4) return 29;
  return 0;
}
