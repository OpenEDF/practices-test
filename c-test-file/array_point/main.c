#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t array_tmp[32] = {0};
char *ch = "this is a test!";

int main(void)
{
    uint64_t array_tmp_addr;
    uint64_t array_tmp_addr_ram;

    array_tmp_addr = (uint64_t)array_tmp;
    printf("array address: 0x%lx\n", array_tmp_addr);

    array_tmp_addr_ram = (uint64_t)&array_tmp;
    printf("array address: 0x%lx\n", array_tmp_addr_ram);

    memcpy((char *)array_tmp_addr, ch, 32);
    for (uint8_t i = 0; i < 32; i++)
    {
        printf("%c", array_tmp[i]);
    }
    printf("\n");

    memcpy((char *)array_tmp_addr_ram, ch, 32);
    for (uint8_t i = 0; i < 32; i++)
    {
        printf("%c", array_tmp[i]);
    }
    printf("\n");
    return 0;
}
