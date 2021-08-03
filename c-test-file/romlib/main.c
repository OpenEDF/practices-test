#include <stdio.h>

typedef struct {
	int (*func1)(int);
	int (*func2)(int);
	int (*func3)(int);
}rom_func_tbl;

int num1 = 1;
int num2 = 2;
int mun3 = 3;


int testfunc1(int num);
int testfunc2(int num);
int testfunc3(int num);
static int testfunc4(void);

rom_func_tbl jump_table __attribute__ ((section(".func_table"))) = {
		.func1 = testfunc1,
	   	.func2 = testfunc2,
	   	.func3 = testfunc3
};

#define func1 jump_table.func1
#define func2 jump_table.func2
#define func3 jump_table.func3

int main(void)
{
	printf("func1(10) = %d.\n", func1(10));
	printf("func2(20) = %d.\n", func2(20));
	printf("func3(30) = %d.\n", func3(30));
	testfunc4();
	return 0;
}

int testfunc1(int num)
{
	return (num * 2);
}

int testfunc2(int num)
{
	return (num * 2);
}

int testfunc3(int num)
{
	return (num * 2);
}

static int testfunc4(void)
{
	printf("This is a test.\n");
	return 0;
}
