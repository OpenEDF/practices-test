#include <stdio.h>

typedef void (*test_str)(int x, int y);
test_str macro_test;

void test1(int y)
{
	printf("The test is from test1 id: %d.\n", y);
}

void test2(int y)
{
	printf("The test is from test2 id: %d.\n", y);
}

/* select func  */
void selet_func(int x, int z)
{
	if (x == 1)
		test1(z);
	else if (x == 0)
		test2(z);
	else
		printf("This is a default test.\n");
}
/* test func */
int main(void)
{
	macro_test = selet_func;
	macro_test(1, 2);
	return 0;	
}

