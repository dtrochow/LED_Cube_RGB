#include <gtest/gtest.h>
#include <iostream>

#include "colors_config.hpp"
#include "led_rgb.hpp"

using namespace std;

class LedRGBTest : public ::testing::Test {
public:
    LedRGBTest();
protected:
    LedRGB lRGB;
};

LedRGBTest::LedRGBTest() : lRGB(ledConfig_getColors()) {}


TEST_F(LedRGBTest, CanSetAndGetRGBColor) {
    lRGB.setColor(Color::RED);
    EXPECT_EQ(lRGB.getColor(), Color::RED);
    lRGB.setColor(Color::GREEN);
    EXPECT_EQ(lRGB.getColor(), Color::GREEN);
    lRGB.setColor(Color::BLUE);
    EXPECT_EQ(lRGB.getColor(), Color::BLUE);
}

TEST_F(LedRGBTest, RGBLedWithRedColor) {
    lRGB.setColor(Color::RED);
    Led_t states = {
        .red = lRGB.getLedDiodeState(Led::RED),
        .green = lRGB.getLedDiodeState(Led::GREEN),
        .blue = lRGB.getLedDiodeState(Led::BLUE)
    };
    Led_t expected_states = {
        .red = LedState::ENABLED,
        .green = LedState::DISABLED,
        .blue = LedState::DISABLED
    };
    EXPECT_EQ(states, expected_states);
}

TEST_F(LedRGBTest, RGBLedWithGreenColor) {
    lRGB.setColor(Color::GREEN);
    Led_t states = {
        .red = lRGB.getLedDiodeState(Led::RED),
        .green = lRGB.getLedDiodeState(Led::GREEN),
        .blue = lRGB.getLedDiodeState(Led::BLUE)
    };
    Led_t expected_states = {
        .red = LedState::DISABLED,
        .green = LedState::ENABLED,
        .blue = LedState::DISABLED
    };
    EXPECT_EQ(states, expected_states);
}

TEST_F(LedRGBTest, RGBLedWithBlueColor) {
    lRGB.setColor(Color::BLUE);
    Led_t states = {
        .red = lRGB.getLedDiodeState(Led::RED),
        .green = lRGB.getLedDiodeState(Led::GREEN),
        .blue = lRGB.getLedDiodeState(Led::BLUE)
    };
    Led_t expected_states = {
        .red = LedState::DISABLED,
        .green = LedState::DISABLED,
        .blue = LedState::ENABLED
    };
    EXPECT_EQ(states, expected_states);
}
