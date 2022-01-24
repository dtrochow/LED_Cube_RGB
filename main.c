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

    // Running second core
    multicore_launch_core1(core1_entry);

    i2c_init(i2c1, I2C_SPEED);
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_set_function(19, GPIO_FUNC_I2C);

    lc_init(i2c1);

    // la_test_all_leds(100);
    // la_planes_animation(70);
    // la_spinning_columns(70);
    la_horizontal_spinning_line(70, LED_CUBE_RGB_Z);
    // la_rnd_led_rnd_color(100);
    
    // uint8_t colors[3] = {RED, GREEN, BLUE};
    // la_rnd_led_rnd_color(200, 100, colors, 3);

    while(true)
    {   
        // for(int i = 0; i < 2000; i += 10)
        // {
        //     la_rnd_led_rnd_color(i, 10, colors, 3);
        // }
        // la_horizontal_spinning_line(70);
    }
}
 