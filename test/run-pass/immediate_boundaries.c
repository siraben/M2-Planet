/* M2's AMD64 int/unsigned are 64-bit machine words, including sizeof(int).
 * Construct high bits at runtime rather than relying on the seed compiler's
 * own int width when parsing a decimal literal. No signed overflow here. */
int calls;
int tick(int n) { calls = calls + 1; return n; }
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int main(void)
{
 int hi; int lo; int x; int y; int *p; int *q; int words[8];
 unsigned u; unsigned high; char *c; char *d; char bytes[8];
 if (sizeof(int) < 8) return 0;
 hi = 2147483647; lo = -2147483647 - 1;
 if (hi + 1 != add(hi, 1)) return 1;
 if (lo - 1 != sub(lo, 1)) return 2;
 if (2147483647 + 1 != add(hi, 1)) return 3;
 if ((-2147483647 - 1) - 1 != sub(lo, 1)) return 4;
 if (2147483647 * 2 != mul(hi, 2)) return 5;
 if (46341 * 46341 != mul(46341, 46341)) return 6;
 if (46340 * 46340 != mul(46340, 46340)) return 7;
 if (-46340 * -46340 != mul(-46340, -46340)) return 8;
 if (-46341 * 46341 != mul(-46341, 46341)) return 9;
 if ((-2147483647 - 1) * -1 != mul(lo, -1)) return 10;
 if ((-2147483647 - 1) * 1 != lo) return 11;
 if (2147483647 - 2147483647 != 0) return 12;
 if (-2147483647 + 2147483647 != 0) return 13;
 u = 1; high = u << 40;
 if ((high + 2147483647) - high != 2147483647) return 14;
 if ((high - 2147483647) + 2147483647 != high) return 15;
 if ((high * 8) / 8 != high) return 16;
 u = 1; u = (u << 32) - 1;
 if ((u & 255) != 255) return 17;
 if ((u ^ 255) + 255 != u) return 18;
 if ((u | 255) != u) return 19;
 if (u < 2147483647) return 20;
 u = 0 - 1;
 if (u * 8 != u - 7) return 21;
 if (u + 1 != 0) return 22;
 x = 10; calls = 0;
 y = x + tick(3);
 if (y != 13 || calls != 1) return 23;
 y = tick(3) + 8;
 if (y != 11 || calls != 2) return 24;
 y = (x + 8) * (tick(2) + 3);
 if (y != 90 || calls != 3) return 25;
 if ((y / 7) != 12 || (y % 7) != 6) return 26;
 if ((y / -7) != -12 || (y % -7) != 6) return 27;
 if ((y >> 2) != 22 || (y << 2) != 360) return 28;
 /* Preserve existing M2 byte offsets for binary pointer +/-, and scaled +=. */
 p = &words; p[0] = 11; p[2] = 33;
 q = p + 2 * sizeof(int);
 if (*q != 33) return 29;
 p += 2;
 q = p - 2 * sizeof(int);
 if (*q != 11) return 30;
 c = &bytes; c[0] = 17; c[2] = 27;
 d = c + 2;
 if (*d != 27) return 31;
 c = c + 2;
 d = c - 2;
 if (*d != 17) return 32;
 if (x + (y = 5) != 15 || y != 5) return 33;
 x = ((high > 8) + (high == 8)) * 3;
 if (x != 3) return 34;
 if (!((high & 255) == 0)) return 35;
 return 0;
}
