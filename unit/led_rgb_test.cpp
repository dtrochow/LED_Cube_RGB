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


TEST_F(LedRGBTest, CanSetRGBColor) {
    lRGB.setColor(Color::RED);
    EXPECT_EQ(lRGB.getColor(), Color::RED);
    lRGB.setColor(Color::GREEN);
    EXPECT_EQ(lRGB.getColor(), Color::GREEN);
    lRGB.setColor(Color::BLUE);
    EXPECT_EQ(lRGB.getColor(), Color::BLUE);
}

// TEST_F(LedRGBTest, RedLedWithRedColor) {
//     lRGB.setColor(lRGB.colors["red"]);
//     LedState state = lRGB.getLedDiodeState(Led::RED);
//     EXPECT_EQ(state, LedState::ENABLED);
// }

// TEST(LedColor, CanCreate) {
//     // LedColor lc(Color::RED, LedState::ENABLED, LedState::DISABLED, LedState::DISABLED);
// }