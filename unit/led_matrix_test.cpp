#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "led_matrix.hpp"
#include "colors_config.hpp"
#include "led_rgb.hpp"
#include "mocks/led_rgb_mock.h"

class LedMatrixTest : public ::testing::Test {
public:
    LedMatrixTest();
protected:

};

LedMatrixTest::LedMatrixTest() {};

TEST_F(LedMatrixTest, CanAssert) {
    ColorDefs colors = ledConfig_getColors();
    LedCreator* factory = new AnalogLedCreator(colors);
    LedMatrix matrix(4, 4, 4, *factory);
    matrix.leds[0][0][0]->setColor(Color::RED);
    EXPECT_EQ(matrix.leds[0][0][3]->getColor(), Color::RED);
}

// TEST_F(LedMatrixTest, CanSetTheMatrixSize) {
//     LedMatrix lMatrix(4, 5, 6, ledConfig_getColors());
//     EXPECT_EQ(lMatrix.getDimension(Dimension::X), 4);
//     EXPECT_EQ(lMatrix.getDimension(Dimension::Y), 5);
//     EXPECT_EQ(lMatrix.getDimension(Dimension::Z), 6);
// }

// TEST_F(LedMatrixTest, CanSetDiodeColor) {
//     LedMatrix lMatrix(4, 4, 4, ledConfig_getColors());
//     lMatrix.leds[0][0][0].setColor(Color::RED);
// }

