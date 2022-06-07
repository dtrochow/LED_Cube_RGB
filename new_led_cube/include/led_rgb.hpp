#pragma once

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

    bool operator==(const Led_t& rh) const {
        return std::make_tuple( this->red, this->green, this->blue ) == std::make_tuple( rh.red, rh.green, rh.blue );
    }
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
