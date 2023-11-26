#include "animation_utils.hpp"

namespace {

std::map<Direction, cartesianPos_t> directions{
    { Direction::X_UP,          { 1,  0,   0   } },
    { Direction::X_DOWN,        { -1, 0,   0   } },
    { Direction::Y_UP,          { 0,  1,   0   } },
    { Direction::Y_DOWN,        { 0,  -1,  0   } },
    { Direction::Z_UP,          { 0,  0,   1   } },
    { Direction::Z_DOWN,        { 0,  0,   -1  } }};

}

cartesianPos_t utils::get_direction(Direction dir)
{
    return directions[dir];
}

/**
 * Inserts random seed from HW for random number generation.
 */
void utils::seed_random(void)
{
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

cubeDim_t utils::get_cube_dimensions(LedCube& cube)
{
    return cubeDim_t{
        .x = cube.getDimension(Dimension::X),
        .y = cube.getDimension(Dimension::Y),
        .z = cube.getDimension(Dimension::Z),
    };
}

Color utils::get_random_color()
{
    utils::seed_random();
    std::initializer_list<Color>::iterator color = all_colors.begin();
    std::advance(color, rand() % all_colors.size() - 1);
    return *color;
}

cartesianPos_t utils::get_random_pos(cubeDim_t cube_dim)
{
    cartesianPos_t pos = { 0 };

    utils::seed_random();
    pos.x = (rand() % (cube_dim.x - 1));
    utils::seed_random();
    pos.y = (rand() % (cube_dim.y - 1));
    utils::seed_random();
    pos.z = (rand() % (cube_dim.z - 1));
    return pos;
}

std::pair<int, int> utils::get_random_xy_pos(cubeDim_t cube_dim)
{
    std::pair<int, int> xy_pos;

    utils::seed_random();
    xy_pos.first = rand() % (cube_dim.x);
    utils::seed_random();
    xy_pos.second = rand() % (cube_dim.y);
    return xy_pos;
}

void utils::render_single_frame(LedCube& cube, MatrixOperation& operation, Color color, uint32_t interval_ms)
{
    cube.action(operation, LedSwitch::ENABLE, color);
    cube.render();
    sleep_ms(interval_ms);
    cube.action(operation, LedSwitch::DISABLE);
    cube.render();
}

void utils::render_single_frame(LedCube& cube, MatrixOperation& operation, Color color)
{
    cube.action(operation, LedSwitch::ENABLE, color);
    cube.render();
    cube.action(operation, LedSwitch::DISABLE);
    cube.render();
}
