#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char c;
    printf("Hello World! Macro on Linux-5.10.0.\n");

    while(1) { 
        c = getchar();
        putchar(c);
    }
    return 0;
}
