#pragma once

#include <map>
#include <initializer_list>
#include "led_rgb_types.hpp"

using AllocatedDiodesMap = std::map<std::pair<int, int>, int>;

struct cartesianPos_t {
    int x;
    int y;
    int z;

    cartesianPos_t(int x = 0, int y = 0, int z = 0)
        : x(x), y(y), z(z) {
    }

    cartesianPos_t& operator=(const cartesianPos_t& a) {
        x = a.x;
        y = a.y;
        z = a.z;
        return *this;
    }

    cartesianPos_t operator+(const cartesianPos_t& a) const {
        return cartesianPos_t(a.x + x, a.y + y, a.z + z);
    }

    bool operator==(const cartesianPos_t& a) const {
        return (x == a.x && y == a.y && z == a.z);
    }
};

struct cubeDim_t{
    int x;
    int y;
    int z;
};

enum class Direction {
    X_UP,
    X_DOWN,
    Y_UP,
    Y_DOWN,
    Z_UP,
    Z_DOWN,
    SIZE
};

constexpr std::initializer_list<Color> all_colors = {
    Color::RED,
    Color::GREEN,
    Color::BLUE,
    Color::CYAN,
    Color::MAGENTA,
    Color::YELLOW,
    Color::WHITE
};

enum class AnimationSpeed : int64_t{
    VERY_SLOW   = 1,
    SLOW        = 2,
    NORMAL      = 3,
    FAST        = 4,
    VERY_FAST   = 5
};
