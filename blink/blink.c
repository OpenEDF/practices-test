/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#define PICO_LED1_PIN 0
#define PICO_LED2_PIN 9
#define PICO_LED3_PIN 13
#define PICO_LED4_PIN 29
#define BLINK_DELAY           100
#define DELAY_S               30
#define LED_BLINK_DELAY_TIME  ((1000 / (BLINK_DELAY * 2)) * DELAY_S)

const uint LED1_PIN = PICO_LED1_PIN;
const uint LED2_PIN = PICO_LED2_PIN;
const uint LED3_PIN = PICO_LED3_PIN;
const uint LED4_PIN = PICO_LED4_PIN;

int main() {

    uint count = 0x00;

    /* init */
    gpio_init(LED1_PIN);
    gpio_init(LED2_PIN);
    gpio_init(LED3_PIN);
    gpio_init(LED4_PIN);

    /* output */
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_set_dir(LED3_PIN, GPIO_OUT);
    gpio_set_dir(LED4_PIN, GPIO_OUT);

    /* blink */
    while (true) {
        do {
            gpio_put(LED1_PIN, 1);
            gpio_put(LED2_PIN, 1);
            gpio_put(LED3_PIN, 1);
            gpio_put(LED4_PIN, 1);

            sleep_ms(BLINK_DELAY);

            gpio_put(LED1_PIN, 0);
            gpio_put(LED2_PIN, 0);
            gpio_put(LED3_PIN, 0);
            gpio_put(LED4_PIN, 0);

            sleep_ms(BLINK_DELAY);
            count++;
        } while (count <= LED_BLINK_DELAY_TIME);

        gpio_put(LED1_PIN, 1);
        gpio_put(LED2_PIN, 1);
        gpio_put(LED3_PIN, 1);
        gpio_put(LED4_PIN, 1);

        while(true);
    }

    return 0;
}
