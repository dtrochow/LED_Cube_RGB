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

    /*
    TESTING
    [X] Different configurations of Single Led
    [X] Different configurations of Planes
    [X] Different configurations of All
    [X] Different configurations of Columns
    [X] Different configurations of Cubiods
    */

    // Enable Single Diode
    for(int i = 0; i < 10; i ++) {
        CartesianCoordinates cr(3, 3, 3);
        EnableSingle enable_single(&cr);
        cube.action(&enable_single, LedSwitch::ENABLE, Color::RED);
        cube.render();
        sleep_ms(200);
        cube.action(&enable_single, LedSwitch::DISABLE);
        cube.render();
        sleep_ms(200);
    }

    // Enable Single Plane
    for(int i = 0; i < 10; i ++) {
        PlaneCoordinates plane(Plane::Z, 2);
        EnablePlane en_plane(&plane);
        cube.action(&en_plane, LedSwitch::ENABLE, Color::CYAN);
        cube.render();
        sleep_ms(200);
        cube.action(&en_plane, LedSwitch::DISABLE);
        cube.render();
        sleep_ms(200);
    }

    // Enable All
    for(int i = 0; i < 10; i ++) {
        CartesianCoordinates cr2(3, 3, 3);
        EnableAll all(&cr2);
        cube.action(&all, LedSwitch::ENABLE, Color::YELLOW);
        cube.render();
        sleep_ms(200);
        cube.action(&all, LedSwitch::DISABLE);
        cube.render();
        sleep_ms(200);
    }

    // Enable Single Column
    for(int i = 0; i < 10; i ++) {
        ColumnCoordinates cl(Plane::Y, 0, 0, 4);
        EnableColumn column(&cl);
        cube.action(&column, LedSwitch::ENABLE, Color::GREEN);
        cube.render();
        sleep_ms(200);
        cube.action(&column, LedSwitch::DISABLE);
        cube.render();
        sleep_ms(200);
    }

    // Enable Single Column
    for(int i = 0; i < 10; i ++) {
        CartesianCoordinates cr3(1, 1, 1);
        CartesianCoordinates cr4(2, 2, 2);
        CuboidCoordinates cb(&cr3, &cr4);
        EnableCuboid cuboid(&cb);
        cube.action(&cuboid, LedSwitch::ENABLE, Color::RED);
        cube.render();
        sleep_ms(200);
        cube.action(&cuboid, LedSwitch::DISABLE);
        cube.render();
        sleep_ms(200);
    }

    while (true) {
    }
}
