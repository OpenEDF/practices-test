#include <stdio.h>

typedef struct {
    int (* test_func)(int x, int y, ...);
} test_t;

test_t test_table;

int test(int xx, int yy, ...)
{
    return (xx + yy);
}

int main(void)
{
    int c = 0;
    test_table.test_func = test;
    c = test_table.test_func(10, 20);
    printf("Hello World! %d\n", c);
    return 0;
}
