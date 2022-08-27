#include "animation_utils.hpp"

static std::map<Direction, cartesianPos_t> directions = {
    { Direction::X_UP,      {1,0,0}},
    { Direction::X_DOWN,    {-1,0,0}},
    { Direction::Y_UP,      {0,1,0}},
    { Direction::Y_DOWN,    {0,-1,0}},
    { Direction::Z_UP,      {0,0,1}},
    { Direction::Z_DOWN,    {0,0,-1}}
};

cartesianPos_t get_direction(Direction dir) {
    return directions[dir];
}

/**
 * Inserts random seed from HW for random number generation.
 */
void seed_random(void) {
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

void get_cube_dimensions(LedCube* cube, cubeDim_t* dimensions) {
    dimensions->x = cube->getDimension(Dimension::X);
    dimensions->y = cube->getDimension(Dimension::Y);
    dimensions->z = cube->getDimension(Dimension::Z);
}

Color get_random_color() {
    seed_random();
    std::initializer_list<Color>::iterator color = all_colors.begin();
    std::advance(color, rand() % all_colors.size()-1);
    return *color;
}

cartesianPos_t get_random_pos(cubeDim_t cube_dim) {
    cartesianPos_t pos = {0};
    seed_random();
    pos.x = rand() % (cube_dim.x-1);
    seed_random();
    pos.y = rand() % (cube_dim.y-1);
    seed_random();
    pos.z = rand() % (cube_dim.z-1);
    return pos;
}

std::pair<int, int> get_random_xy_pos(cubeDim_t cube_dim) {
    std::pair<int, int> xy_pos;
    seed_random();
    xy_pos.first = rand() % (cube_dim.x);
    seed_random();
    xy_pos.second = rand() % (cube_dim.y);
    return xy_pos;
}
