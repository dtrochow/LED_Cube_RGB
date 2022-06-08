#include "led_rgb_types.hpp"

#pragma once

class LedColor {
public:
    LedColor(Color color_, LedState red, LedState green, LedState blue);
    ~LedColor();
    Color getLedColor();
    Led_t getLedStates();
protected:
    Color color;
    Led_t led_states;
};

class LedRGB {
public:
    LedRGB(std::map<Color, LedColor> colors_);
    ~LedRGB();
    void setColor(Color color_);
    Color getColor();
    LedState getLedDiodeState(Led led);
    void disable();
    void enable();
protected:
    Color color_before_disable;
    Color color;
    Led_t led_states;
    std::map<Color, LedColor> available_colors;
private:
    LedColor getColorObj(Color color_);
};
