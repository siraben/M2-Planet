/* Copyright (C) 2026 Ben Siraphob
 * This file is part of M2-Planet.
 *
 * M2-Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * M2-Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2-Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Regression test, verifies that integer literal suffixes are ignored
 * when parsing instead of making strtoint reject the whole token and
 * silently yield zero.
 */

#define MASK 0xFU

#if 2U > 1
#define SUFFIXED_IF_WORKS 1
#else
#define SUFFIXED_IF_WORKS 0
#endif

enum { COUNT = 3u };

int sized[COUNT];

int cases(int x)
{
	switch(x)
	{
		case 7U: return 70;
		case 11UL: return 110;
		default: return 0;
	}
}

int main()
{
	int a = 42U;
	if(a != 42) return 1;

	long b = 42L;
	if(b != 42) return 2;

	unsigned c = 0xFFul;
	if(c != 255) return 3;

	int d = 077LU;
	if(d != 63) return 4;

	int e = 'A';
	if(e != 65) return 5;

	if(cases(7) != 70) return 6;
	if(cases(11) != 110) return 7;
	if(cases(12) != 0) return 8;

	sized[2] = 9;
	if(sized[2] != 9) return 9;

	int m = MASK;
	if(m != 15) return 10;

	if(SUFFIXED_IF_WORKS != 1) return 11;

	return 0;
}
