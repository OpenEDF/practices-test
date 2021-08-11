#include <stdio.h>
#include <stdint.h>

uint8_t test_byte = 0x56;   /* 0101 0110 */
uint8_t array_bits[8] = {0};

/* convern 1 Byte to 8 bits */
void bytetobits(uint8_t byte_value)
{
    for (uint8_t index = 0; index < 8; index++)
    {
        array_bits[index] = (byte_value >> index) & 0x01;
        printf("\rarray_bits[%d] = 0x%02X.\n", index, array_bits[index]);
    }
}

/* 8 bits to 1 byte */
uint8_t bitstobyte(uint8_t *data)
{
    uint8_t temp = 0x00;
    for(uint8_t index = 0; index < 8; index++)
    {
        temp |= data[index] << index;
    }

    printf("\rtemp = 0x%02X.\n", temp);
}

int main(void)
{
    bytetobits(test_byte);
    bitstobyte(array_bits);
    return 0;
}

/*
 * compiler and test
 * $ gcc main.c
 * $ ./a.out
 *
 * result:
 * array_bits[0] = 0x00.
 * array_bits[1] = 0x01.
 * array_bits[2] = 0x01.
 * array_bits[3] = 0x00.
 * array_bits[4] = 0x01.
 * array_bits[5] = 0x00.
 * array_bits[6] = 0x01.
 * array_bits[7] = 0x00.
 * temp = 0x56.
 * */
