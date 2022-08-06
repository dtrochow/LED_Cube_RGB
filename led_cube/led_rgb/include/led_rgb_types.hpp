#pragma once
#include <cinttypes>
#include <tuple>

enum class Color : std::uint8_t {
    NONE,
    RED,
    GREEN,
    BLUE,
    CYAN,
    MAGENTA,
    YELLOW,
    WHITE
};

enum class Led : std::uint8_t {
    RED = 0,
    GREEN,
    BLUE
};

enum class LedState : bool {
    DISABLED,
    ENABLED
};

enum class LedSwitch : bool {
    ENABLE,
    DISABLE
};

struct Led_t {
    LedState red;
    LedState green;
    LedState blue;

    bool operator==(const Led_t& rh) const {
        return std::make_tuple( this->red, this->green, this->blue ) == std::make_tuple( rh.red, rh.green, rh.blue );
    }
};
