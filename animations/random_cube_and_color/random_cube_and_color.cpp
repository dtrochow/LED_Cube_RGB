#include "random_cube_and_color.hpp"

/**
 * Enables random cube with random color, size and position.
 * 1. Get position of the cube (the lowest corner - the closest to (0,0,0)). The only forbidden coordinates are
 *    the coordinates of the last led (size_x-1, size_y-1, size_z-1)
 * 2. Get size of the cube depending on available space.
 *
 *    e.g. - the position drawn is (1,0,2) - marked by (X)
 *         - forbidden coordinates are marked by (F)
 *         - second coordinates od the cube are marked by (S)
 *
 *            (0,3,3) -->   o - o - o -(F)   <-- (3,3,3)
 *                         /   /   /   /|
 *                        o - o - o - o |
 *                       /   /   /   /|/|
 *                      o - o -(S)- o / |
 *                     /   /   /   /|/|/|
 *      (0,0,3) -->   o - o - o - o / / |   <-- (3,3,0)
 *                    |   |   |   |/|/|/
 *                    o -(X)- o - o / /
 *                    |   |   |   |/|/
 *                    o - o - o - o /
 *                    |   |   |   |/
 *      (0,0,0) -->   o - o - o - o   <-- (3,0,0)
 *
 *      It means that the only available size is 2 (size_z - pos.z), because the cubes will be created in
 *      a positive direction, and the side of cube can not be outside LedMatrix.
 *      So the only possible coordinates for this cube will be ((1,0,2),(2,1,3))
 * 3. Get the color of the cube
 * 4. Draw cube
 */
void RandomCubeAndColor::run(LedCube& cube, const AnimationSpeed speed, const int iterations)
{
    // Define speed of the animation
    constexpr uint32_t base_interval = 1000;
    const uint32_t interval = (base_interval / (2 * utils::to_underlying(speed)));
    const cubeDim_t cube_dimensions = utils::get_cube_dimensions(cube);

    // Define number of iteration
    for (int i = 0; i < iterations; i++) {
        // 1. Get position of the cube
        const cartesianPos_t pos = utils::get_random_pos(cube_dimensions);

        // 2. Get size of the cube depending on available space.
        const int cube_size = std::min((cube_dimensions.x - pos.x), std::min((cube_dimensions.y - pos.y), (cube_dimensions.z - pos.z)));

        // 3. Get the color
        const Color color = utils::get_random_color();

        // 4. Draw the cube
        XYZCoordinates X(pos.x, pos.y, pos.z);
        XYZCoordinates S((pos.x + (cube_size - 1)),
                               (pos.y + (cube_size - 1)),
                               (pos.z + (cube_size - 1)));
        MatrixOperation enable_cuboid = EnableCuboid(CuboidCoordinates(X, S));

        cube.reset();
        cube.action(enable_cuboid, LedSwitch::ENABLE, color);
        cube.render();
        sleep_ms(interval);
    }

    cube.reset();
    cube.render();
}
