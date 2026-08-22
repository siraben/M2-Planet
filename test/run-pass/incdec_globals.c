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

/* Regression test, verifies that prefix/postfix inc/dec work on
 * globals and statics (which previously crashed by dereferencing the
 * loaded value as an address) and that inc/dec after an index
 * operation increments the element instead of stepping by the
 * pointer's object size.
 */

int g;
char c;
int arr[4];

int main()
{
	g = 10;
	if(g++ != 10) return 1;
	if(g != 11) return 2;
	if(++g != 12) return 3;
	if(g-- != 12) return 4;
	if(--g != 10) return 5;

	c = 30;
	c++;
	if(c != 31) return 6;

	int local[2];
	local[0] = 7;
	local[0]++;
	if(local[0] != 8) return 7;
	--local[0];
	if(local[0] != 7) return 8;

	arr[2] = 5;
	arr[2]++;
	if(arr[2] != 6) return 9;
	if(++arr[2] != 7) return 10;

	static int counter;
	counter = 3;
	counter++;
	if(counter != 4) return 11;

	return 0;
}
