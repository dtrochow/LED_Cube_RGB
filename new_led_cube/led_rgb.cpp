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

LedRGBAnalog::LedRGBAnalog(ColorDefs colors_config) {
    available_colors = colors_config;
    Led_t all_disabled = {
        .red = LedState::DISABLED,
        .green = LedState::DISABLED,
        .blue = LedState::DISABLED
    };
    led_states = all_disabled;
}

LedRGBAnalog::~LedRGBAnalog() {}

void LedRGBAnalog::setColor(Color color_) {
    LedColor colorObj = getColorObj(color_);
    color = colorObj.getLedColor();
    led_states = colorObj.getLedStates();
}

Color LedRGBAnalog::getColor() {
    return color;
}

LedState LedRGBAnalog::getLedDiodeState(Led led_) {
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

LedColor LedRGBAnalog::getColorObj(Color color_) {
    auto it = available_colors.find(color_);
    return it->second;
}

void LedRGBAnalog::disable() {
    color_before_disable = color;
    Led_t all_disabled = {
        .red = LedState::DISABLED,
        .green = LedState::DISABLED,
        .blue = LedState::DISABLED
    };
    color = Color::NONE;
    led_states = all_disabled;
}

void LedRGBAnalog::enable() {
    setColor(color_before_disable);
}

LedRGB* LedCreator::MakeLed() const {
    LedRGB* led = this->CreateMethod();
    return led;
}

AnalogLedCreator::AnalogLedCreator(ColorDefs color_config) {
    colors = color_config;
}

LedRGB* AnalogLedCreator::CreateMethod() const {
    return new LedRGBAnalog(colors);
}
