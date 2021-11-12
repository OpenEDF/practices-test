#include <stdio.h>
#include <stdint.h>

void test_a(void);
int test_b(void);
void exchange_address_a(uint32_t *address_a, uint32_t *address_b);
void exchange_address_b(void (*address_a)(void), int (*address_b)(void));
void exchange_address_c(void *address_a, void *address_b);
typedef void(* void_address_func)(void);
typedef int (* int_address_func)(void);
void exchange_address_d(void_address_func address_a, int_address_func address_b);

int main(int argc, char** argv)
{

   /* get the function address */
   exchange_address_a(test_a, test_b);
   exchange_address_b(test_a, test_b);
   exchange_address_c(test_a, test_b);

   exchange_address_d(test_a, test_b);

   return 0;
}

void test_a(void)
{
   printf("The is a test form test1 !\n");
}

int test_b(void)
{
   printf("The is a test form test2 !\n");
   return 0x11223344;
}

void exchange_address_a(uint32_t *address_a, uint32_t *address_b)
{
   uint32_t adda = address_a;
   uint32_t addb = address_b;
   printf("The function a address is 0x%x\n", adda);
   printf("The function b address is 0x%x\n", addb);
}

void exchange_address_b(void (*address_a)(void), int (*address_b)(void))
{
   uint32_t adda = address_a;
   uint32_t addb = address_b;
   printf("The function a address is 0x%x\n", adda);
   printf("The function b address is 0x%x\n", addb);
}

void exchange_address_c(void *address_a, void *address_b)
{
   uint32_t adda = (uint32_t)address_a;
   uint32_t addb = (uint32_t)address_b;
   printf("The function a address is 0x%x\n", adda);
   printf("The function b address is 0x%x\n", addb);
}

void exchange_address_d(void_address_func address_a, int_address_func address_b)
{
   uint32_t adda = (uint32_t)address_a;
   uint32_t addb = (uint32_t)address_b;
   printf("The function a address is 0x%x\n", adda);
   printf("The function b address is 0x%x\n", addb);
}
