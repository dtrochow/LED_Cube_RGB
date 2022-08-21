#include "led_rgb_types.hpp"
#include <initializer_list>

constexpr std::initializer_list<Color> all_colors = {
    Color::RED,
    Color::GREEN,
    Color::BLUE,
    Color::CYAN,
    Color::MAGENTA,
    Color::YELLOW,
    Color::WHITE
};

enum class AnimationType {
    ALL_LEDS_ALL_COLORS,
    RANDOM_CUBE_AND_COLOR,
    RAIN,
    RAISING_COLUMNS,
    SNAKE,
    NONE
};

enum class AnimationSpeed {
    VERY_SLOW = 1,
    SLOW,
    NORMAL,
    FAST,
    VERY_FAST
};

struct cartesianPos_t {
    int x;
    int y;
    int z;

    cartesianPos_t(int x=0, int y=0, int z=0) 
        : x(x), y(y), z(z)
    {
    }

    cartesianPos_t& operator=(const cartesianPos_t& a) {
        x=a.x;
        y=a.y;
        z=a.z;
        return *this;
    }

    cartesianPos_t operator+(const cartesianPos_t& a) const {
        return cartesianPos_t(a.x+x, a.y+y, a.z+z);
    }

    bool operator==(const cartesianPos_t& a) const {
        return (x == a.x && y == a.y && z == a.z);
    }
};

typedef struct {
    int x;
    int y;
    int z;
} cubeDim_t;

enum class Direction {
    X_UP,
    X_DOWN,
    Y_UP,
    Y_DOWN,
    Z_UP,
    Z_DOWN,
    SIZE
};
