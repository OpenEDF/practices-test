#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct test_str {
    uint8_t *addr;
    uint16_t len;
} test_str_t;

void test_fun(test_str_t *pkg, uint8_t cnt)
{
    for (uint8_t i = 0; i < cnt; i++) {
        printf("0x%p\n", pkg->addr);
        printf("0x%x\n", pkg->len);
       // printf("0x%p\n", pkg[i]->addr); //ERROR
       // printf("0x%x\n", pkg[i]->len);  //ERROR
        free(pkg->addr);
        pkg++;
    }
}

int main(void)
{
    uint8_t *pkg_addr1 = NULL;
    uint8_t *pkg_addr2 = NULL;
    uint8_t *pkg_addr3 = NULL;
    test_str_t pre_pkg[3];

    pkg_addr1 = (uint8_t *)malloc(10);
    pkg_addr2 = (uint8_t *)malloc(10);
    pkg_addr3 = (uint8_t *)malloc(10);

    pre_pkg[0].addr = pkg_addr1;
    pre_pkg[0].len = 10;

    pre_pkg[1].addr = pkg_addr2;
    pre_pkg[1].len = 10;

    pre_pkg[2].addr = pkg_addr3;
    pre_pkg[2].len = 10;

    printf("The pkg addr1: 0x%p addr2: 0x%p addr3: 0x%p\n", pkg_addr1, pkg_addr2, pkg_addr3);
    test_fun(&pre_pkg[0], 3);
    return 0;
}
