#include <stdio.h>
#include <stdint.h>

char test_string[10] = {0};

int main(int argc, char** argv)
{
   float x = 156.78f;
   float y = x * 100.0f;
   uint32_t z;
   uint32_t temp;

   z = (uint32_t)y;
   printf("z = %d\n", z);

   test_string[0] = z / 10000 + 0x30;
   temp = z % 10000;

   test_string[1] = temp / 1000 + 0x30;
   temp = temp % 1000;

   test_string[2] = temp / 100 + 0x30;

   test_string[3] = 0x2E;           /* . */

   temp = z % 100;
   test_string[4] = temp / 10 + 0x30;

   test_string[5] = temp % 10 + 0x30;
   printf("x = %s\n", test_string);
}
