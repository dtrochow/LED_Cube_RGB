#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include "animation_runner.hpp"
#include "animations.hpp"

/* -------------------------------------------------------------------------- */
/*                              Animations Runner                             */
/* -------------------------------------------------------------------------- */

AnimationsRunner::AnimationsRunner(LedCube* cube_) {
    cube = cube_;
    animations = {
		{ AnimationType::RANDOM_CUBE_AND_COLOR, new RandomCubeAndColor() },
		{ AnimationType::ALL_LEDS_ALL_COLORS, new AllLedsAllColors() },
        { AnimationType::RAIN, new Rain() },
        { AnimationType::RAISING_COLUMNS, new RaisingColumns() },
        { AnimationType::SNAKE, new Snake() }
    };
}

void AnimationsRunner::run(AnimationType a_type, AnimationSpeed speed, int iterations) {
    animations[a_type]->run(cube, speed, iterations);
}

/* -------------------------------------------------------------------------- */
/*                                 Animations                                 */
/* -------------------------------------------------------------------------- */

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
            xy_pos = get_random_xy_pos(cube_dimensions);
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

/**
 * Snake animation
 */
void Snake::run(LedCube* cube, AnimationSpeed speed, int iterations) {
    get_cube_dimensions(cube, &cube_dimensions);
    one_frame_time_us = 300000/(int64_t)speed;

    dir = drawFirstDirection();
    drawStartingPoint();

    while (true) {
        last_dir = dir;
        last_pos = allocated_diodes.back();

        if (isHitTheWall() || isChangeDirection()) {
            auto possible_dirs = getAllCurrentPossibleDirs();
            dir = drawDirection(possible_dirs);
        } else {
            dir = last_dir;
        }

        moveSnake(dir);
        render(cube);
    };
}

bool Snake::isHitTheWall() {
    return ((last_pos.x == 0 && last_dir == Direction::X_DOWN) ||
            (last_pos.x == cube_dimensions.x-1 && last_dir == Direction::X_UP) ||
            (last_pos.y == 0 && last_dir == Direction::Y_DOWN) ||
            (last_pos.y == cube_dimensions.y-1  && last_dir == Direction::Y_UP) ||
            (last_pos.z == 0 && last_dir == Direction::Z_DOWN) ||
            (last_pos.z == cube_dimensions.y-1 && last_dir == Direction::Z_UP));
}

bool Snake::isChangeDirection() {
    seed_random();
    return (bool(rand() % 2));
}

void Snake::drawStartingPoint() {
    start_pos = get_random_pos(cube_dimensions);
    allocated_diodes.push_back({start_pos.x, start_pos.y, start_pos.z});
    sleep_ms(one_frame_time_us/1000);
}

Direction Snake::drawFirstDirection() {
    seed_random();
    return (Direction)(rand() % (int)Direction::SIZE);
}

std::vector<Direction> Snake::getAllCurrentPossibleDirs() {
    std::vector<Direction> allowed_dir;
    cartesianPos_t potential_next_pos;

    if ((last_pos.x != 0) && (last_dir != Direction::X_UP)) {
        allowed_dir.push_back(Direction::X_DOWN);
    }
    if ((last_pos.x != cube_dimensions.x-1) && (last_dir != Direction::X_DOWN)) {
        allowed_dir.push_back(Direction::X_UP);
    }
    if ((last_pos.y != 0) && (last_dir != Direction::Y_UP)) {
        allowed_dir.push_back(Direction::Y_DOWN);
    }
    if ((last_pos.y != cube_dimensions.y-1) && (last_dir != Direction::Y_DOWN)) {
        allowed_dir.push_back(Direction::Y_UP);
    }
    if ((last_pos.z != 0) && (last_dir != Direction::Z_UP)) {
        allowed_dir.push_back(Direction::Z_DOWN);
    } 
    if ((last_pos.z != cube_dimensions.z-1) && (last_dir != Direction::Z_DOWN)) {
        allowed_dir.push_back(Direction::Z_UP);
    }
    // Remove possibility of running into the snake tail
    for (auto it_dir = allowed_dir.begin(); it_dir != allowed_dir.end();) {
        potential_next_pos = last_pos + get_direction(*it_dir);
        if (std::count(allocated_diodes.begin(), allocated_diodes.end(), potential_next_pos)) {
            it_dir = allowed_dir.erase(it_dir);
        } else {
            it_dir++;
        }
    }
    return allowed_dir;
}

Direction Snake::drawDirection(std::vector<Direction> dirs) {
    auto it_dir = dirs.begin();
    seed_random();
    std::advance(it_dir, rand() % (dirs.size()));
    return *it_dir;
}

void Snake::moveSnake(Direction dir) {
    cartesianPos_t next_pos = last_pos + get_direction(dir);
    allocated_diodes.push_back(next_pos);
    if (allocated_diodes.size() > snake_length) {
        allocated_diodes.erase(allocated_diodes.begin());
    }
}

void Snake::render(LedCube* cube) {
    absolute_time_t start = get_absolute_time();
    while (absolute_time_diff_us(start, get_absolute_time()) < one_frame_time_us) {
        int tail_pos = 0;
        auto it_color = all_colors.begin();
        for (auto it = allocated_diodes.begin(); it != allocated_diodes.end(); it++) {
            cartesianPos_t pos = *it;
            CartesianCoordinates cc(pos.x, pos.y, pos.z);
            EnableSingle single(&cc);
            cube->action(&single, LedSwitch::ENABLE, *it_color);
            cube->render();
            cube->action(&single, LedSwitch::DISABLE);
            cube->render();
            tail_pos++;
            // Dividing the tail into separate color sections
            if (tail_pos % color_segment_length == 0) {
                it_color++;
            }
        }
    }; 
}
