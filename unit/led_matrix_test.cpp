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

MockLedRGB* GetSingleLedMock(int x, int y, int z, LedRGB3DMatrix matrix) {
    // return (MockLedRGB*)matrix[x][y][z];
    return (MockLedRGB*)matrix[x];
}

class LedMatrixTest : public ::testing::Test {
public:
    LedMatrixTest();
protected:
    LedCreator* ledFactory = new MockLedCreator();
};

LedMatrixTest::LedMatrixTest() {};

// 1. Make method for filling all matrix fields with LedRGB objects
// 2. Make helper method for destroying mock objects
// 3. Make helper method for getting all mock objects, to use them in tests
// 4. Helper method for iterating through all leds
// 5. Enable column
// 6. Enaple row
// 7. Enable plane
// 8. Enable single led

TEST_F(LedMatrixTest, CanAssert) {
    LedMatrix* matrix = new LedMatrix(4, 4, 4, *ledFactory);
    MockLedRGB* led_mock = GetSingleLedMock(0, 0, 0, matrix->leds);
    EXPECT_CALL(*led_mock, enable()).Times(1);
    EXPECT_CALL(*led_mock, Die()).Times(1);
    matrix->enableAll();
    delete led_mock;
}

TEST_F(LedMatrixTest, CanSetTheMatrixSize) {
    LedMatrix lMatrix(4, 5, 6, *ledFactory);
    EXPECT_EQ(lMatrix.getDimension(Dimension::X), 4);
    EXPECT_EQ(lMatrix.getDimension(Dimension::Y), 5);
    EXPECT_EQ(lMatrix.getDimension(Dimension::Z), 6);
}
