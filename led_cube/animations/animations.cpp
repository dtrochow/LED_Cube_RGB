#include <stdlib.h>
#include "animations.hpp"
#include "led_matrix.hpp"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"

#include "hardware/timer.h"

/* -------------------------------------------------------------------------- */
/*                             Static definitions                             */
/* -------------------------------------------------------------------------- */

static void seed_random(void);
static void get_cube_dimensions(LedCube* cube, cubeDim_t* dimensions);

/* -------------------------------------------------------------------------- */

/**
 * Inserts random seed from HW for random number generation.
 */
static void seed_random(void) {
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

static void get_cube_dimensions(LedCube* cube, cubeDim_t* dimensions) {
    dimensions->x = cube->getDimension(Dimension::X);
    dimensions->y = cube->getDimension(Dimension::Y);
    dimensions->z = cube->getDimension(Dimension::Z);
}

/* -------------------------------------------------------------------------- */
/*                                 Enable page                                */
/* -------------------------------------------------------------------------- */

typedef std::pair<CartesianCoordinates, Color> LedDesc;

void enable_page(LedCube* cube, LedDesc led_descriptors, int duration_ms)
{

}


/* -------------------------------------------------------------------------- */
/*                              Animations Runner                             */
/* -------------------------------------------------------------------------- */

AnimationsRunner::AnimationsRunner(LedCube* cube_) {
    cube = cube_;
    animations = {
        { AnimationType::ALL_LEDS_ALL_COLORS,   new AllLedsAllColors()  },
        { AnimationType::RANDOM_CUBE_AND_COLOR, new RandomCubeAndColor() },
        { AnimationType::RAIN, new Rain() },
        { AnimationType::CHESS_CUBES, new ChessCubes() }
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
 *    e.g. - the position drwan is (1,0,2) - marked by (I)
 *         - forbidden coordinates are marked by (F)
 *         - second coordinates od the cube are marked by (I)                         
 *              
 *            (0,3,3) -->   o - o - o -(F)   <-- (3,3,3)
 *                         /   /   /   /|  
 *                        o - o - o - o |  
 *                       /   /   /   /|/|
 *                      o - o -(I)- o / |     
 *                     /   /   /   /|/|/|  
 *      (0,0,3) -->   o - o - o - o / / |   <-- (3,3,0)    
 *                    |   |   |   |/|/|/   
 *                    o -(I)- o - o / /     
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
        cartesianPos_t pos = {0};
        seed_random();
        pos.x = rand() % (cube_dimensions.x-1);
        seed_random();
        pos.y = rand() % (cube_dimensions.y-1);
        seed_random();
        pos.z = rand() % (cube_dimensions.z-1);

        // 2. Get size of the cube depending on available space.
        cube_size = std::min((cube_dimensions.x-pos.x), std::min((cube_dimensions.y-pos.y), (cube_dimensions.z-pos.z)));

        // 3. Get the color
        seed_random();
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
            seed_random();
            xy_pos.first = rand() % (cube_dimensions.x);
            seed_random();
            xy_pos.second = rand() % (cube_dimensions.y);
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

/**
 * ChessCubes animation
 * 
 *            (0,3,3) -->  (O)-(O)-(I)-(I)  <-- (3,3,3)
 *                         /   /   /   /|  
 *                       (O)-(O)-(I)-(I)|  
 *                       /   /   /   /|/|
 *                     (I)-(I)-(O)-(O)/ |     
 *                     /   /   /   /|/|/|  
 *      (0,0,3) -->  (I)-(I)-(O)-(O)/ / |   <-- (3,3,0)    
 *                    |   |   |   |/|/|/   
 *                   (I)-(I)-(O)-(O)/ /     
 *                    |   |   |   |/|/       
 *                   (O)-(O)-(I)-(I)/       
 *                    |   |   |   |/ 
 *      (0,0,0) -->  (O)-(O)-(I)-(I)  <-- (3,0,0)
 * 
 * The color will change during each iteration. There always will be two colors (O) and (I) as shown
 * on the ilustration above.
 */
void ChessCubes::run(LedCube* cube, AnimationSpeed speed, int iterations) {

}

