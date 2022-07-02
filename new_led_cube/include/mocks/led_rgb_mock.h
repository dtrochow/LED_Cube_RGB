#include <gmock/gmock.h>

#include "led_rgb.hpp"


class MockLedRGB : public LedRGB {
public:
    MOCK_METHOD(void, setColor, (Color color_));
    MOCK_METHOD(Color, getColor, ());
    MOCK_METHOD(LedState, getLedDiodeState, (Led led));
    MOCK_METHOD(void, disable, ());
    MOCK_METHOD(void, enable, ());
    MOCK_METHOD(void, Die, ());
    ~MockLedRGB() override { Die(); }
};
