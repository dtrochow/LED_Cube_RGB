#include "random_cube_and_color.hpp"

/**
 * Enables random cube with random color, size and position.
 * 1. Get position of the cube (the lowest corner - the closest to (0,0,0)). The only forbidden coordinates are
 *    the coordinates of the last led (size_x-1, size_y-1, size_z-1)
 * 2. Get size of the cube depending on available space. 
 * 
 *    e.g. - the position drwan is (1,0,2) - marked by (X)
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
void RandomCubeAndColor::run(LedCube* cube, AnimationSpeed speed, int iterations) {
    // Define speed of the animation
    uint32_t base_interval = 1000;
    uint32_t interval = base_interval/(2*(int)speed);

    int cube_size = 0;
    cubeDim_t cube_dimensions = {0};

    // Get cube dimensions
    get_cube_dimensions(cube, &cube_dimensions);

    // Define number of iteration
    for (int i = 0; i < iterations; i ++) {
        // 1. Get position of the cube
        cartesianPos_t pos = get_random_pos(cube_dimensions);

        // 2. Get size of the cube depending on available space.
        cube_size = std::min((cube_dimensions.x-pos.x), std::min((cube_dimensions.y-pos.y), (cube_dimensions.z-pos.z)));

        // 3. Get the color
        Color color = get_random_color();
        
        // 4. Draw the cube
        CartesianCoordinates X(pos.x, pos.y, pos.z);
        CartesianCoordinates S(pos.x+(cube_size-1), pos.y+(cube_size-1), pos.z+(cube_size-1));
        CuboidCoordinates cb(&X, &S);
        EnableCuboid cuboid(&cb);
        cube->reset();
        cube->action(&cuboid, LedSwitch::ENABLE, color);
        cube->render();
        sleep_ms(interval);
    }
    cube->reset();
    cube->render();
}
