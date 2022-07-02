#pragma once

#include "led_rgb_types.hpp"

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
    virtual ~LedRGB() {};
    virtual void enable() {};
    virtual void disable() {};
    virtual void setColor(Color color_) {};
    virtual Color getColor() = 0;
    virtual LedState getLedDiodeState(Led led) = 0;
};

typedef std::map<Color, LedColor> ColorDefs;

class LedRGBAnalog : public LedRGB {
public:
    LedRGBAnalog(ColorDefs colors_config);
    ~LedRGBAnalog() {};
    void setColor(Color color_) override;
    Color getColor() override;
    LedState getLedDiodeState(Led led) override;
    void disable() override;
    void enable() override;
protected:
    Color color_before_disable;
    Color color;
    Led_t led_states;
    ColorDefs available_colors;
private:
    LedColor getColorObj(Color color_);
};

class LedCreator {
public:
    virtual ~LedCreator() {}
    virtual LedRGB* CreateMethod() const = 0;
    LedRGB* MakeLed() const;
};

class AnalogLedCreator : public LedCreator{
public:
    AnalogLedCreator(ColorDefs color_config);
    LedRGB* CreateMethod() const override;
protected:
    ColorDefs colors;
};
