#include <stdlib.h>
#include "animations.hpp"
#include "led_matrix.hpp"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"

/* -------------------------------------------------------------------------- */
/*                             Static definitions                             */
/* -------------------------------------------------------------------------- */

static void la_seed_random_from_rosc(void);

/* -------------------------------------------------------------------------- */

/**
 * Inserts random seed from HW for random number generation.
 */
static void la_seed_random_from_rosc(void) {
    uint32_t random = 0x811c9dc5;
    uint8_t next_byte = 0;
    volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);

    for (int i = 0; i < 16; i++) {
        for (int k = 0; k < 8; k++) {
            next_byte = (next_byte << 1) | (*rnd_reg & 1);
        }

        random ^= next_byte;
        random *= 0x01000193;
    }

    srand(random);
}

/* -------------------------------------------------------------------------- */
/*                              Animations Runner                             */
/* -------------------------------------------------------------------------- */

AnimationsRunner::AnimationsRunner(LedCube* cube_) {
    cube = cube_;
    animations = {
        { AnimationType::ALL_LEDS_ALL_COLORS,   new AllLedsAllColors()  },
        { AnimationType::RANDOM_CUBE_AND_COLOR, new RandomCubeAndColor() }
    };
}

void AnimationsRunner::run(AnimationType a_type, AnimationSpeed speed, int iterations) {
    animations[a_type]->run(cube, speed, iterations);
}

/* -------------------------------------------------------------------------- */
/*                                 Animations                                 */
/* -------------------------------------------------------------------------- */

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
    cube_dimensions.x = cube->getDimension(Dimension::X);
    cube_dimensions.y = cube->getDimension(Dimension::Y);
    cube_dimensions.z = cube->getDimension(Dimension::Z);

    // Define number of iteration
    for (int i = 0; i < iterations; i ++) {
        // 1. Get position of the cube
        cartesianPos_t pos = {0};
        la_seed_random_from_rosc();
        pos.x = rand() % (cube_dimensions.x-1);
        la_seed_random_from_rosc();
        pos.y = rand() % (cube_dimensions.y-1);
        la_seed_random_from_rosc();
        pos.z = rand() % (cube_dimensions.z-1);

        // 2. Get size of the cube depending on available space.
        cube_size = std::min((cube_dimensions.x-pos.x), std::min((cube_dimensions.y-pos.y), (cube_dimensions.z-pos.z)));

        // 3. Get the color
        la_seed_random_from_rosc();
        std::initializer_list<Color>::iterator color = all_colors.begin();
        std::advance(color, rand() % all_colors.size()-1);
        
        // 4. Draw the cube
        CartesianCoordinates X(pos.x, pos.y, pos.z);
        CartesianCoordinates S(pos.x+(cube_size-1), pos.y+(cube_size-1), pos.z+(cube_size-1));
        CuboidCoordinates cb(&X, &S);
        EnableCuboid cuboid(&cb);
        cube->reset();
        cube->action(&cuboid, LedSwitch::ENABLE, *color);
        cube->render();
        sleep_ms(interval);
    }
    cube->reset();
    cube->render();
}