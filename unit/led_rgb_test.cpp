#include <gtest/gtest.h>

#include "colors_config.hpp"
#include "led_rgb.hpp"


class LedRGBTest : public ::testing::Test {
public:
    LedRGBTest();
protected:
    LedRGB lRGB;
};

LedRGBTest::LedRGBTest() : lRGB(ledConfig_getColors()) {}

Led_t getLedStates(LedRGB led_rgb) {
    Led_t states = {
        .red = led_rgb.getLedDiodeState(Led::RED),
        .green = led_rgb.getLedDiodeState(Led::GREEN),
        .blue = led_rgb.getLedDiodeState(Led::BLUE)
    };
    return states;
}

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
    Led_t states = getLedStates(lRGB);
    Led_t expected_states = {
        .red = LedState::ENABLED,
        .green = LedState::DISABLED,
        .blue = LedState::DISABLED
    };
    EXPECT_EQ(states, expected_states);
}

TEST_F(LedRGBTest, RGBLedWithGreenColor) {
    lRGB.setColor(Color::GREEN);
    Led_t states = getLedStates(lRGB);
    Led_t expected_states = {
        .red = LedState::DISABLED,
        .green = LedState::ENABLED,
        .blue = LedState::DISABLED
    };
    EXPECT_EQ(states, expected_states);
}

TEST_F(LedRGBTest, RGBLedWithBlueColor) {
    lRGB.setColor(Color::BLUE);
    Led_t states = getLedStates(lRGB);
    Led_t expected_states = {
        .red = LedState::DISABLED,
        .green = LedState::DISABLED,
        .blue = LedState::ENABLED
    };
    EXPECT_EQ(states, expected_states);
}

TEST_F(LedRGBTest, RGBLedWithCyanColor) {
    lRGB.setColor(Color::CYAN);
    Led_t states = getLedStates(lRGB);
    Led_t expected_states = {
        .red = LedState::DISABLED,
        .green = LedState::ENABLED,
        .blue = LedState::ENABLED
    };
    EXPECT_EQ(states, expected_states);
}

TEST_F(LedRGBTest, RGBLedWithMagentaColor) {
    lRGB.setColor(Color::MAGENTA);
    Led_t states = getLedStates(lRGB);
    Led_t expected_states = {
        .red = LedState::ENABLED,
        .green = LedState::DISABLED,
        .blue = LedState::ENABLED
    };
    EXPECT_EQ(states, expected_states);
}

TEST_F(LedRGBTest, RGBLedWithYellowColor) {
    lRGB.setColor(Color::YELLOW);
    Led_t states = getLedStates(lRGB);
    Led_t expected_states = {
        .red = LedState::ENABLED,
        .green = LedState::ENABLED,
        .blue = LedState::DISABLED
    };
    EXPECT_EQ(states, expected_states);
}

TEST_F(LedRGBTest, RGBLedWithWhiteColor) {
    lRGB.setColor(Color::WHITE);
    Led_t states = getLedStates(lRGB);
    Led_t expected_states = {
        .red = LedState::ENABLED,
        .green = LedState::ENABLED,
        .blue = LedState::ENABLED
    };
    EXPECT_EQ(states, expected_states);
}

TEST_F(LedRGBTest, InitiallyAllLedsDisabled) {
    Led_t states = getLedStates(lRGB);
    Led_t expected_states = {
        .red = LedState::DISABLED,
        .green = LedState::DISABLED,
        .blue = LedState::DISABLED
    };
    EXPECT_EQ(states, expected_states);
}

TEST_F(LedRGBTest, CanDisableWholeRGBLed) {
    lRGB.setColor(Color::WHITE);
    Led_t enable_states = getLedStates(lRGB);
    Led_t enable_expected_states = {
        .red = LedState::ENABLED,
        .green = LedState::ENABLED,
        .blue = LedState::ENABLED
    };
    lRGB.disable();
    EXPECT_EQ(enable_states, enable_expected_states);
    Led_t disable_states = getLedStates(lRGB);
    Led_t disable_expected_states = {
        .red = LedState::DISABLED,
        .green = LedState::DISABLED,
        .blue = LedState::DISABLED
    };
    EXPECT_EQ(disable_states, disable_expected_states);
    Color color = lRGB.getColor();
    EXPECT_EQ(color, Color::NONE);
}

TEST_F(LedRGBTest, CanEnableWholeRGBLedWithPreviousState) {
    lRGB.setColor(Color::CYAN);
    Led_t cyan_states = getLedStates(lRGB);
    Led_t cyan_expected_states = {
        .red = LedState::DISABLED,
        .green = LedState::ENABLED,
        .blue = LedState::ENABLED
    };
    lRGB.disable();
    EXPECT_EQ(cyan_states, cyan_expected_states);
    Led_t disable_states = getLedStates(lRGB);
    Led_t disable_expected_states = {
        .red = LedState::DISABLED,
        .green = LedState::DISABLED,
        .blue = LedState::DISABLED
    };
    EXPECT_EQ(disable_states, disable_expected_states);
    lRGB.enable();
    Led_t enable_states = getLedStates(lRGB);
    EXPECT_EQ(enable_states, cyan_expected_states);
    Color color = lRGB.getColor();
    EXPECT_EQ(color, Color::CYAN);
}
