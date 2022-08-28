#include "raising_columns.hpp"

/**
 * Raising Columns animation
 */
void RaisingColumns::run(LedCube* cube, AnimationSpeed speed, int iterations) {
    int64_t one_frame_time = 300000/(int64_t)speed;
    cubeDim_t cube_dimensions = {0};
    get_cube_dimensions(cube, &cube_dimensions);
    std::map<std::pair<int, int>, std::pair<ColumnCoordinates, Color>> allocated_columns;
    std::map<std::pair<int,int>, std::pair<ColumnCoordinates, Color>>::iterator it = allocated_columns.end();

    while(true) {
        // 1. Randomly get one XY diode position
        std::pair<int, int> xy_pos;
        do {
            seed_random();
            xy_pos.first = rand() % (cube_dimensions.x);
            seed_random();
            xy_pos.second = rand() % (cube_dimensions.y);
            it = allocated_columns.find(xy_pos);
        } while (it != allocated_columns.end());

        // 2. Iterate thorugh all allocated columns and increment height
        for (it = allocated_columns.begin(); it != allocated_columns.end();) {
            // Deallocate columns if height value == MAX_Z
            if (it->second.first.height== cube_dimensions.z) {
                it = allocated_columns.erase(it);
            }
            else {
                it->second.first.height++;
                it++;
            }
        }

        // 3. Allocate random column from (1.) by adding it to map and set its height to 1
        allocated_columns.insert({xy_pos, {ColumnCoordinates(Plane::Z, xy_pos.first, xy_pos.second, 1), get_random_color()}});

        // 4. Render the columns from a map
        absolute_time_t start = get_absolute_time();
        while (absolute_time_diff_us(start, get_absolute_time()) < one_frame_time) {
            for (it = allocated_columns.begin(); it != allocated_columns.end(); it++) {
                std::pair<int, int> xy = it->first;
                EnableColumn single(&it->second.first);
                cube->action(&single, LedSwitch::ENABLE, it->second.second);
                cube->render();
                cube->action(&single, LedSwitch::DISABLE);
                cube->render();
            }
        };
    }
}
