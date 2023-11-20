#pragma once

#include <map>
#include <memory>

#include "led_rgb_types.hpp"

class LedColor;

using ColorDefs = std::map<Color, LedColor>;

class LedColor {
public:
    LedColor(Color color_, LedState red, LedState green, LedState blue);
    ~LedColor() = default;
    Color getLedColor();
    Led_t getLedStates();
private:
    Color color;
    Led_t led_states;
};

class LedRGB {
public:
    virtual ~LedRGB() = default;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual void setColor(Color color_) = 0;
    virtual Color getColor() = 0;
    virtual LedState getLedDiodeState(Led led) = 0;
};

class LedRGBAnalog : public LedRGB {
public:
    LedRGBAnalog(ColorDefs colors_config);
    void setColor(Color color_) override;
    Color getColor() override;
    LedState getLedDiodeState(Led led) override;
    void disable() override;
    void enable() override;
private:
    Color color_before_disable;
    Color color;
    Led_t led_states;
    ColorDefs available_colors;
    LedColor getColorObj(Color color_);
};

class LedCreator {
public:
    virtual ~LedCreator() = default;
    virtual std::unique_ptr<LedRGB> MakeLed() const = 0;
};

class AnalogLedCreator : public LedCreator {
public:
    AnalogLedCreator(ColorDefs color_config);
    std::unique_ptr<LedRGB> MakeLed() const override;
private:
    ColorDefs colors;
};
