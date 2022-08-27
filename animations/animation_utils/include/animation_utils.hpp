#pragma once

#include <stdlib.h>
#include "animation_utils_types.hpp"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"
#include "led_cube.hpp"

class Animation {
public:
    virtual ~Animation() {};
public:
    /**
     * @param cube - pointer to a LedCube object
     * @param speed  - one of 5 available speed options for animation
     * @param iterations - number of animation iterations
     */
    virtual void run(LedCube* cube, AnimationSpeed speed, int iterations) = 0;
};

void seed_random(void);
void get_cube_dimensions(LedCube* cube, cubeDim_t* dimensions);
Color get_random_color();
cartesianPos_t get_random_pos(cubeDim_t cube_dim);
std::pair<int, int> get_random_xy_pos(cubeDim_t cube_dim);
cartesianPos_t get_direction(Direction dir);
