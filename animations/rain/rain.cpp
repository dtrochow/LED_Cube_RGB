#include "rain.hpp"

/**
 * Rain animation
 */
void Rain::run(LedCube* cube, AnimationSpeed speed, int iterations) {
    int64_t one_frame_time = 300000/(int64_t)speed;
    cubeDim_t cube_dimensions = {0};
    get_cube_dimensions(cube, &cube_dimensions);
    std::map<std::pair<int, int>, int> allocated_diodes;
    std::map<std::pair<int,int>,int>::iterator it = allocated_diodes.end();

    // Choosing color of the rain
    Color color = Color::CYAN;

    while(true) {
        // 1. Randomly get one XY diode position
        std::pair<int, int> xy_pos;
        do {
            xy_pos = get_random_xy_pos(cube_dimensions);
            it = allocated_diodes.find(xy_pos);
        } while (it != allocated_diodes.end());

        // 2. Iterate thorugh all allocated diodes and decrement Z value
        for (it = allocated_diodes.begin(); it != allocated_diodes.end();) {
            // Deallocate diode if Z value == 0
            if (it->second == 0) {
                it = allocated_diodes.erase(it);
            }
            else {
                it->second--;
                it++;
            }
        }

        // 3. Allocate random diode from (1.) by adding it to map and set its Z value to maximum
        allocated_diodes.insert({xy_pos, cube_dimensions.z-1});

        // 4. Render the diodes from a map
        absolute_time_t start = get_absolute_time();
        while (absolute_time_diff_us(start, get_absolute_time()) < one_frame_time) {
            for (it = allocated_diodes.begin(); it != allocated_diodes.end(); it++) {
                std::pair<int, int> xy = it->first;
                CartesianCoordinates cc(xy.first, xy.second, it->second);
                EnableSingle single(&cc);
                cube->action(&single, LedSwitch::ENABLE, color);
                cube->render();
                cube->action(&single, LedSwitch::DISABLE);
                cube->render();
            }
        };
    }
}
