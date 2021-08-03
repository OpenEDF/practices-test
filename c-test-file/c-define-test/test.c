#include <stdio.h>

#define __FUNT__(A,B)   \
  int c = A + B; 	    \
  printf("%d\n", c)		

#define __TEST__()	    \
  int c = 1 + 2 +       \
      3 + 4 +           \
      5 + 6;            \
  printf("%d\n", c)

void tessst(int x, int y)
{
    __FUNT__(x, y);
}


int main(int argc, char** argv)
{
  tessst(20, 30);
  __TEST__();
  printf("This is a test program.\n");
  return 0;
}  
