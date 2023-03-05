#include <stdio.h>

int value = 0x00;

int main(void)
{
    printf("gdb watchpoint test:\n");
    /* loop */
    for(int index = 0x00; index < 0x0A; index++) {
        printf("value = %d\n", value);
        value++;
    }
    return 0;
}

/*
 * compiler:
 * gcc -g watch
 *
 * run:
 * gdb ./a.our
 * (gdb) watch value
 * (gdb) r
 * (gdb)
 * gdb watchpoint test:
 * value = 0
 * 
 *  Hardware watchpoint 1: value
 *  Old value = 0
 *  New value = 1
 *  main () at watchp.c:10
 *  10          for(int index = 0x00; index < 10; index++) {
 * */
