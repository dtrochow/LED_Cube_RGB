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

typedef struct {
    int x;
    int y;
    int z;
} cartesianPos_t;

typedef struct {
    int x;
    int y;
    int z;
} cubeDim_t;
