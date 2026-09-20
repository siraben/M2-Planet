int main(void)
{
	int a = -1;
	int b = 1;
	unsigned u = 0 - 1;
	unsigned v = 1;
	int saved;

	/* Signed/unsigned ordering and equality in branch conditions. */
	if(a >= b) return 1;
	if(a > b) return 2;
	if(b <= a) return 3;
	if(b < a) return 4;
	if(a == b) return 5;
	if(a != a) return 6;
	if(u <= v) return 7;
	if(u < v) return 8;
	if(v >= u) return 9;
	if(v > u) return 10;

	/* A loop takes the opposite branch polarity. */
	do { a = a + 1; } while(a < b);
	if(a != b) return 11;

	/* A stored comparison still produces a Boolean value. */
	saved = u > v;
	if(saved != 1) return 12;
	return 0;
}
