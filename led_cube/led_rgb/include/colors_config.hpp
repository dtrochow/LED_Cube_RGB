#pragma once

#include <map>
#include "led_rgb.hpp"

static std::map<Color, LedColor> led_colors = {
    { Color::RED,     LedColor(Color::RED,     LedState::ENABLED,       LedState::DISABLED,        LedState::DISABLED)  },
    { Color::GREEN,   LedColor(Color::GREEN,   LedState::DISABLED,      LedState::ENABLED,         LedState::DISABLED)  },
    { Color::BLUE,    LedColor(Color::BLUE,    LedState::DISABLED,      LedState::DISABLED,        LedState::ENABLED)   },
    { Color::CYAN,    LedColor(Color::CYAN,    LedState::DISABLED,      LedState::ENABLED,         LedState::ENABLED)   },
    { Color::MAGENTA, LedColor(Color::MAGENTA, LedState::ENABLED,       LedState::DISABLED,        LedState::ENABLED)   },
    { Color::YELLOW,  LedColor(Color::YELLOW,  LedState::ENABLED,       LedState::ENABLED,         LedState::DISABLED)  },
    { Color::WHITE,   LedColor(Color::WHITE,   LedState::ENABLED,       LedState::ENABLED,         LedState::ENABLED)   },
};

std::map<Color, LedColor> ledConfig_getColors() {
    return led_colors;
}
