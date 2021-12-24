/***************************************
 * main.c
 * rev 1.0 Dominik 2021
 *  ************************************/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "hardware/i2c.h"s
#include "led_cube.h"

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

    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_set_function(19, GPIO_FUNC_I2C);

    mcp_enable(ADDRESS1);
    mcp_enable(ADDRESS2);
    lc_enable_diode(0, 0, 0, RED, true);

    mcp_set_mode(i2c1, ADDRESS1, GPIOB, 0x00);
    mcp_write(i2c1, ADDRESS1, GPIOB, 0x00, true);
    mcp_init();
    mcp_write(i2c1, ADDRESS1, GPIOA, 0xFE, true);
    mcp_write_single(i2c1, ADDRESS1, 15, HIGH, true);
    mcp_get_output_value(ADDRESS1);
    mcp_update_out_state(i2c1, ADDRESS1);
    mcp_update_out_state_all(i2c1);

    uint8_t state[8] = {0x06, 0x04, 0x00, 0x02, 0x06, 0x04, 0x02, 0x00};
    uint8_t port_value;

    int i = 0;
    int led_state = 0;
    while(true)
    {
        if(i >= sizeof(state))
        {
            i = 0;
        }
        led_state ^= 1;
        gpio_put(LED_PIN, led_state);
        mcp_write(i2c1, ADDRESS1, GPIOB, state[i], true);
        i ++;
        sleep_ms(200);
        port_value = mcp_get_output_value_port(ADDRESS1, GPIOB);
        printf("Port B value: %d", port_value);
    }
}
 