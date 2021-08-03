int adder(int x, int y)
{
	int a = 0;
	if (x > 10)
		goto testpointer;
	a = x + y;
	return a;
testpointer:
	return x;
}
