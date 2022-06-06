#include <iostream>
#include <map>
#include "led_rgb.hpp"


LedColor::LedColor(Color color_, LedState red, LedState green, LedState blue) {
    color = color_;
    led_states.red = red;
    led_states.green = green;
    led_states.blue = blue;
}
LedColor::~LedColor() {}

Led_t LedColor::getLedStates() {
    return led_states;
}

Color LedColor::getLedColor() {
    return color;
}

LedRGB::LedRGB(std::map<Color, LedColor> colors_) {
    available_colors = colors_;
    Led_t all_disabled = {
        .red = LedState::DISABLED,
        .green = LedState::DISABLED,
        .blue = LedState::DISABLED
    };
    led_states = all_disabled;
}

LedRGB::~LedRGB() {}

void LedRGB::setColor(Color color_) {
    LedColor colorObj = getColorObj(color_);
    color = colorObj.getLedColor();
    led_states = colorObj.getLedStates();
}

Color LedRGB::getColor() {
    return color;
}

LedState LedRGB::getLedDiodeState(Led led_) {
    switch(led_) {
        case Led::RED:
            return led_states.red;
        break;
        case Led::GREEN:
            return led_states.green;
        break;
        case Led::BLUE:
            return led_states.blue;
        break;
    }
}

LedColor LedRGB::getColorObj(Color color_) {
    auto it = available_colors.find(color_);
    return it->second;
}

void LedRGB::disable() {
    color_before_disable = color;
    Led_t all_disabled = {
        .red = LedState::DISABLED,
        .green = LedState::DISABLED,
        .blue = LedState::DISABLED
    };
    color = Color::NONE;
    led_states = all_disabled;
}

void LedRGB::enable() {
    setColor(color_before_disable);
}
