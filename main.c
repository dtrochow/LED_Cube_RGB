/***************************************
 * main.c
 * rev 1.0 Dominik 2021
 *  ************************************/

#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/led_cube.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "hardware/i2c.h"

const uint LED_PIN = 25;

#define ADDRESS 0x20
#define ADDRESS1 0x21
#define IODIRA 0x00
#define IODIRB 0x01
#define GPIOA 0x12
#define GPIOB 0x13

// Pin set as output - 0, input - 1

void core1_entry()
{
    while(1)
    {

    }
}

int main(void)
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    multicore_launch_core1(core1_entry);

    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_set_function(19, GPIO_FUNC_I2C);
    
    uint8_t buf[2];

    buf[0] = IODIRA;
    buf[1] = 0x00;
    // i2c_write_blocking(i2c1, ADDRESS, buf, 2, false);
    i2c_write_blocking(i2c1, ADDRESS1, buf, 2, false);

    buf[0] = IODIRB;
    buf[1] = 0x00;
    // i2c_write_blocking(i2c1, ADDRESS, buf, 2, false);
    i2c_write_blocking(i2c1, ADDRESS1, buf, 2, false);

    buf[0] = GPIOA;
    buf[1] = 0xFE;
    // i2c_write_blocking(i2c1, ADDRESS, buf, 2, false);
    i2c_write_blocking(i2c1, ADDRESS1, buf, 2, false);

    buf[0] = GPIOB;
    buf[1] = 0xFE;
    // i2c_write_blocking(i2c1, ADDRESS, buf, 2, false);
    i2c_write_blocking(i2c1, ADDRESS1, buf, 2, false);

    uint8_t state[8] = {0x06, 0x04, 0x00, 0x02, 0x06, 0x04, 0x02, 0x00};

    buf[0] = GPIOB;

    int i = 0;
    while(true)
    {
        gpio_put(LED_PIN, 1);
        buf[1] = state[i];
        // i2c_write_blocking(i2c1, ADDRESS, buf, 2, false);
        i2c_write_blocking(i2c1, ADDRESS1, buf, 2, false);
        i ++;
        if(i >= 7)
            i = 0;
        sleep_ms(400);
        gpio_put(LED_PIN, 0);
        buf[1] = state[i];
        // i2c_write_blocking(i2c1, ADDRESS, buf, 2, false);
        i2c_write_blocking(i2c1, ADDRESS1, buf, 2, false);
        i ++;
        if(i >= 7)
            i = 0;
        sleep_ms(400);
    }
}
 