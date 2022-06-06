#include <iostream>

#pragma once

enum class Color : std::uint8_t {
    RED,
    GREEN,
    BLUE
};

enum class Led : std::uint8_t {
    RED,
    GREEN,
    BLUE
};

enum class LedState : bool {
    DISABLED,
    ENABLED
};

struct Led_t {
    LedState red;
    LedState green;
    LedState blue;
};

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
protected:
    Color color;
    Led_t led_states;
    std::map<Color, LedColor> available_colors;
private:
    LedColor getColorObj(Color color_);
};
