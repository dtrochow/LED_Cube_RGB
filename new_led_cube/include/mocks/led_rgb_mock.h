#include <gmock/gmock.h>

#include "led_rgb.hpp"

// class LedRGB {
// public:
//     LedRGB(std::map<Color, LedColor> colors_);
//     ~LedRGB();
//     void setColor(Color color_);
//     Color getColor();
//     LedState getLedDiodeState(Led led);
//     void disable();
//     void enable();
// protected:
//     Color color_before_disable;
//     Color color;
//     Led_t led_states;
//     std::map<Color, LedColor> available_colors;
// private:
//     LedColor getColorObj(Color color_);
// };

class MockLedRGB : public LedRGB {
 public:
  MOCK_METHOD(Color, getColor, ());
};
