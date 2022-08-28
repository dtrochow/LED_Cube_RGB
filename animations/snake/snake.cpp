#include <algorithm>
#include "snake.hpp"

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
