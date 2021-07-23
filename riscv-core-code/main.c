#include "wq70xx.h"

void software_init(void);
/* main.c */
void main(void)
{
    GPIO_BASEADDR |= (1 << 2);
    I2C0_BASEADDR &= ~(1 << 1);
    while(1);

}

void software_init(void)
{
    return;
}