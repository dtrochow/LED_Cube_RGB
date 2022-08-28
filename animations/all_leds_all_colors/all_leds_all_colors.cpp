#include "all_leds_all_colors.hpp"

/**
 * Enables all leds one by one with all available colors.
 */
void AllLedsAllColors::run(LedCube* cube, AnimationSpeed speed, int iterations) {
    for (auto color: all_colors) {
        for (int z = 0; z < cube->getDimension(Dimension::Z); z++) {
            for (int y = 0; y < cube->getDimension(Dimension::Y); y++) {
                for (int x = 0; x < cube->getDimension(Dimension::X); x++) {
                    CartesianCoordinates cr(x, y, z);
                    EnableSingle enable_single(&cr);
                    cube->action(&enable_single, LedSwitch::ENABLE, color);
                    cube->render();
                    sleep_ms(100);
                    cube->action(&enable_single, LedSwitch::DISABLE);
                    cube->render();
                }
            }
        }
    }
}
