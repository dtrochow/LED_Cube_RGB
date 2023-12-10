#include <algorithm>

#include "all_leds_all_colors.hpp"
#include "animation_utils.hpp"

namespace {

void performActions(LedCube& cube, const Color color, XYZCoordinates cr, AnimationSpeed speed)
{
    constexpr uint32_t base_interval_ms = 1000;
    const uint32_t interval_ms = (base_interval_ms / static_cast<uint32_t>(utils::to_underlying(speed)));

    MatrixOperation enable_single = EnableSingle(cr);
    utils::render_single_frame(cube, enable_single, color, interval_ms);
}

}

/**
 * Enables all leds one by one with all available colors.
 */
void AllLedsAllColors::run(LedCube& cube, const AnimationSpeed speed, const int iterations)
{
    for (int z = 0; z < cube.getDimension(Dimension::Z); z++) {
        for (int y = 0; y < cube.getDimension(Dimension::Y); y++) {
            for (int x = 0; x < cube.getDimension(Dimension::X); x++) {
                std::for_each(all_colors.begin(), all_colors.end(),
                              [&](const Color& color) {
                    performActions(cube, color, XYZCoordinates(x, y, z), speed);
                });
            }
        }
    }
}
