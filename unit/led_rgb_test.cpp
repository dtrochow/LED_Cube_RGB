#include <gtest/gtest.h>

#include <iostream>
#include "led_rgb.hpp"

using namespace std;

class LedRGBTest : public ::testing::Test {
public:

protected:
    LedRGB lRGB;
};

TEST_F(LedRGBTest, CanSetRGBColor) {
    lRGB.setColor(Color::RED);
    lRGB.setColor(Color::GREEN);
    lRGB.setColor(Color::BLUE);
}
