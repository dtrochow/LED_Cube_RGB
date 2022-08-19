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
#include "animations.hpp"

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

    AnimationsRunner a_runner(&cube);
    a_runner.run(AnimationType::ALL_LEDS_ALL_COLORS);
    a_runner.run(AnimationType::RANDOM_CUBE_AND_COLOR, AnimationSpeed::FAST, 400);
    a_runner.run(AnimationType::RAIN, AnimationSpeed::VERY_SLOW);
    a_runner.run(AnimationType::CHESS_CUBES);

    while (true) {
    }
}
