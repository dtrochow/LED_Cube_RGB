#pragma once

#include <stdlib.h>
#include "animation_utils_types.hpp"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"
#include "led_cube.hpp"

class Animation {
public:
    virtual ~Animation() = default;
public:
    /**
     * @param cube - pointer to a LedCube object
     * @param speed  - one of 5 available speed options for animation
     * @param iterations - number of animation iterations
     */
    virtual void run(LedCube& cube, const AnimationSpeed speed, const int iterations) = 0;
};

namespace utils
{

template <typename EnumType>
constexpr typename std::underlying_type<EnumType>::type to_underlying(EnumType value)
{
    return static_cast<std::underlying_type_t<EnumType> >(value);
}

void seed_random(void);
cubeDim_t get_cube_dimensions(LedCube& cube);
Color get_random_color();
cartesianPos_t get_random_pos(cubeDim_t cube_dim);
std::pair<int, int> get_random_xy_pos(cubeDim_t cube_dim);
cartesianPos_t get_direction(Direction dir);
void render_single_frame(LedCube& cube, MatrixOperation& operation, Color color, uint32_t interval);
void render_single_frame(LedCube& cube, MatrixOperation& operation, Color color);

}