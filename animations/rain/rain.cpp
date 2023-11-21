#include "rain.hpp"

/**
 * Rain animation
 */
void Rain::run(LedCube& cube, const AnimationSpeed speed, const int iterations) {
    const int64_t one_frame_time = (300000 / utils::to_underlying(speed));
    const cubeDim_t cube_dimensions = utils::get_cube_dimensions(cube);

    AllocatedDiodesMap allocated_diodes{};
    auto it = allocated_diodes.end();

    // Choosing color of the rain
    const Color rain_color = Color::CYAN;

    while (true) {
        // 1. Randomly get one XY diode position
        std::pair<int, int> xy_pos{};
        do {
            xy_pos = utils::get_random_xy_pos(cube_dimensions);
            it = allocated_diodes.find(xy_pos);
        } while (it != allocated_diodes.end());

        // 2. Iterate through all allocated diodes and decrement Z value
        for (it = allocated_diodes.begin(); it != allocated_diodes.end();) {
            // Deallocate diode if Z value == 0
            if (it->second == 0) {
                it = allocated_diodes.erase(it);
            } else {
                it->second--;
                it++;
            }
        }

        // 3. Allocate random diode from (1.) by adding it to map and set its Z value to maximum
        allocated_diodes.insert({ xy_pos, cube_dimensions.z - 1 });

        // 4. Render the diodes from a map
        const absolute_time_t start = get_absolute_time();

        while (absolute_time_diff_us(start, get_absolute_time()) < one_frame_time) {
            for (it = allocated_diodes.begin(); it != allocated_diodes.end(); it++) {
                const std::pair<int, int> xy = it->first;

                MatrixOperation enable_single = EnableSingle(CartesianCoordinates(xy.first, xy.second, it->second));
                utils::render_single_frame(cube, enable_single, rain_color);
            }
        };
    }
}
