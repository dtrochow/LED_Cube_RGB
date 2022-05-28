/***************************************
* main.c
* rev 1.0 Dominik 2021
*  ************************************/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "hardware/i2c.h"
#include "led_animations_api.h"

const int I2C_SPEED = 1000000;
const uint LED_PIN = 25;

void core1_entry() {
    while (1) {
    }
}

int main(void) {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Running second core
    multicore_launch_core1(core1_entry);

    i2c_init(i2c1, I2C_SPEED);
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_set_function(19, GPIO_FUNC_I2C);

    lc_init(i2c1);

    while (true) {
    }
}
