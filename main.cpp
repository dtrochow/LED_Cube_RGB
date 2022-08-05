/***************************************
* main.cpp
* rev 1.0 Dominik 2022
*  ************************************/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "hardware/i2c.h"

#include "led_cube.hpp"
#include "led_matrix.hpp"
#include "memory_hub.hpp"
#include "led_rgb.hpp"
#include "colors_config.hpp"

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

    AnalogLedCreator ledFactory(ledConfig_getColors());
    AnalogLedCubeRGB4x4x4MCP23017 memory_hub(i2c1);
    LedCubeAnalog4x4x4 cube(&memory_hub, &ledFactory);

    // 1. EnableAll needs to be fixed. For now it needs greater coordinates than possible.

    // Enable Single Diode
    CartesianCoordinates cr(3, 3, 3);
    EnableSingle enable_single(&cr);
    cube.action(&enable_single, LedSwitch::ENABLE, Color::RED);
    cube.render();
    sleep_ms(2000);
    cube.action(&enable_single, LedSwitch::DISABLE);
    cube.render();
    sleep_ms(2000);

    // Enable Plane
    PlaneCoordinates plane(Plane::Z, 2);
    EnablePlane en_plane(&plane);
    cube.action(&en_plane, LedSwitch::ENABLE, Color::CYAN);
    cube.render();
    sleep_ms(2000);
    cube.action(&en_plane, LedSwitch::DISABLE);
    cube.render();
    
    while (true) {
    }
}