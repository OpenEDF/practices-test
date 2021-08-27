#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char** argv)
{
   uint32_t length;

   /* check the parameters */
   if (argc != 3)
   {
        printf("[ERROR] Input parameter is incaild. Please re-enter it in the following format.\n");
        printf("str2byte \"string\" size\n");
        printf("[NOTE] size cannot be larger than 9.\n");
        return -1;
   }

   length = strlen(argv[1]);

   /* print the string length */
   printf("The string length is: %d\n", length);

   for (uint32_t index = 0; index < length; index++)
   {
      printf("string[%d]= %c\t", index, argv[1][index]);

      if ((index + 1) % (*argv[2] - 0x30) == 0)
         printf("\n");
   }
   printf("\n");

   return 0;
}
