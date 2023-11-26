#include "raising_columns.hpp"

namespace
{

struct ColumnDescriptor {
    ColumnCoordinates column_coordinates;
    Color color;
};

using AllocatedColumnsMap = std::map<CartesianCoordinates, ColumnDescriptor>;

}

/**
 * Raising Columns animation
 */
void RaisingColumns::run(LedCube& cube, const AnimationSpeed speed, const int iterations)
{
    const int64_t one_frame_time = (300000 / utils::to_underlying(speed));
    const cubeDim_t cube_dimensions = utils::get_cube_dimensions(cube);

    AllocatedColumnsMap allocated_columns{};
    auto it = allocated_columns.end();

    while (true) {
        // 1. Randomly get one XY diode position
        CartesianCoordinates xy_pos(0, 0, 0);
        do {
            utils::seed_random();
            xy_pos.x = rand() % (cube_dimensions.x);
            utils::seed_random();
            xy_pos.y = rand() % (cube_dimensions.y);
            it = allocated_columns.find(xy_pos);
        } while (it != allocated_columns.end());

        // 2. Iterate through all allocated columns and increment height
        for (it = allocated_columns.begin(); it != allocated_columns.end();) {
            // Deallocate columns if height value == MAX_Z
            if (it->second.column_coordinates.height == cube_dimensions.z) {
                it = allocated_columns.erase(it);
            } else {
                it->second.column_coordinates.height++;
                it++;
            }
        }

        // 3. Allocate random column from (1.) by adding it to map and set its height to 1
        allocated_columns.insert({ xy_pos, { ColumnCoordinates(Plane::Z, xy_pos.x, xy_pos.y, 1), utils::get_random_color() } });

        // 4. Render the columns from a map
        absolute_time_t start = get_absolute_time();

        while (absolute_time_diff_us(start, get_absolute_time()) < one_frame_time) {
            for (it = allocated_columns.begin(); it != allocated_columns.end(); it++) {

                MatrixOperation enable_column = EnableColumn(it->second.column_coordinates);
                utils::render_single_frame(cube, enable_column, it->second.color);
            }
        }
        ;
    }
}
