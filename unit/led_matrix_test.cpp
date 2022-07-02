#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "led_matrix.hpp"
#include "led_rgb.hpp"
#include "mocks/led_rgb_mock.h"

using ::testing::Return;

class MockLedCreator : public LedCreator {
public:
    MockLedCreator() {};
    LedRGB* CreateMethod() const override {
        return new MockLedRGB;
    }
};

class LedMatrixTest : public ::testing::Test {
public:
    LedMatrixTest();
protected:
    LedCreator* ledFactory = new MockLedCreator();
};

LedMatrixTest::LedMatrixTest() {};

TEST_F(LedMatrixTest, CanAssert) {
    // auto Led = std::make_shared<MockLedRGB>();
    LedMatrix matrix(4, 4, 4, *ledFactory);
    MockLedRGB* led = (MockLedRGB*)matrix.leds[0][0][0];
    EXPECT_CALL(*led, enable());

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

