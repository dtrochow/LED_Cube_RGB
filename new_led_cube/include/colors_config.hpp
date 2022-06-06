#include <iostream>
#include <map>
#include "led_rgb.hpp"

#pragma once

static std::map<Color, LedColor> led_colors = {
    { Color::RED, LedColor(Color::RED, LedState::ENABLED, LedState::DISABLED, LedState::DISABLED) },
    { Color::GREEN, LedColor(Color::GREEN, LedState::DISABLED, LedState::ENABLED, LedState::DISABLED) },
    { Color::BLUE, LedColor(Color::BLUE, LedState::DISABLED, LedState::DISABLED, LedState::ENABLED) }
};

std::map<Color, LedColor> ledConfig_getColors() {
    return led_colors;
}
