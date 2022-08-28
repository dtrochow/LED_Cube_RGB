#include <algorithm>
#include "snake.hpp"

Snake::Snake() {
    next_color = get_random_color();
}

/**
 * Snake animation
 */
void Snake::run(LedCube* cube, AnimationSpeed speed, int iterations) {
    get_cube_dimensions(cube, &cube_dimensions);
    one_frame_time_us = 300000/(int64_t)speed;
    init_action_matrix();

    dir = drawFirstDirection();
    drawStartingPoint();

    while (true) {
        last_dir = dir;
        last_pos = allocated_diodes.front().first;

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

void Snake::init_action_matrix() {
    action_matrix.resize(cube_dimensions.x, std::vector<vector<MatrixOperation*>>(cube_dimensions.y, std::vector<MatrixOperation*>(cube_dimensions.z, nullptr)));
    action_matrix[1][1][1] = new EnableSingle(new CartesianCoordinates(1,1,1));
    for (int x = 0; x < cube_dimensions.x; x++) {
        for (int y = 0; y < cube_dimensions.y; y++) {
            for (int z = 0; z < cube_dimensions.z; z++) {
                action_matrix[x][y][z] = new EnableSingle(new CartesianCoordinates(x, y, z));
            }
        }
    }
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
    allocated_diodes.push_back({{start_pos.x, start_pos.y, start_pos.z}, next_color});
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
        bool if_intersect = (std::find_if(allocated_diodes.begin(), allocated_diodes.end(), 
                             [&potential_next_pos](const std::pair<cartesianPos_t, Color> &diode)
                             {return potential_next_pos == diode.first;}) != allocated_diodes.end());
        if_intersect ? it_dir = allowed_dir.erase(it_dir) : it_dir++;
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
    allocated_diodes.insert(allocated_diodes.begin(), {next_pos, next_color});
    if (allocated_diodes.size() > snake_length) {
        allocated_diodes.erase(allocated_diodes.end());
    }
    snake_move_counter++;
    if (snake_move_counter % color_segment_length == 0) {
        next_color = get_random_color();
    }
}

void Snake::render(LedCube* cube) {
    absolute_time_t start = get_absolute_time();
    while (absolute_time_diff_us(start, get_absolute_time()) < one_frame_time_us) {
        for (auto it = allocated_diodes.begin(); it != allocated_diodes.end(); it++) {
            cartesianPos_t pos = it->first;
            cube->action(action_matrix[pos.x][pos.y][pos.z], LedSwitch::ENABLE, it->second);
            cube->render();
            cube->action(action_matrix[pos.x][pos.y][pos.z], LedSwitch::DISABLE);
            cube->render();
        }
    };
}
