#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "led_matrix.hpp"
#include "led_rgb.hpp"
#include "led_rgb_mock.h"

using ::testing::Return;

class MockLedCreator : public LedCreator {
public:
    MockLedCreator() {};
    std::unique_ptr<LedRGB> MakeLed() const override
    {
        return std::make_unique<MockLedRGB>();
    }

};

class LedMatrixTest : public ::testing::Test {
public:
    void SetUp() override {};

    void TearDown() override {};

    LedMatrixTest() {};
    MockLedRGB* GetSingleLedMock(int x, int y, int z, LedRGB3DMatrix &matrix)
    {
        return dynamic_cast<MockLedRGB*>(matrix[x][y][z].get());
    }

protected:
    std::unique_ptr<LedCreator> ledFactory = std::make_unique<MockLedCreator>();
};

TEST_F(LedMatrixTest, CanSetTheMatrixSize) {
    LedMatrix lMatrix(4, 5, 6, *ledFactory);

    EXPECT_EQ(lMatrix.getDimension(Dimension::X), 4);
    EXPECT_EQ(lMatrix.getDimension(Dimension::Y), 5);
    EXPECT_EQ(lMatrix.getDimension(Dimension::Z), 6);
}

TEST_F(LedMatrixTest, CanEnableAllLedsInArray) {
    LedMatrix matrix(4, 5, 6, *ledFactory);

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                MockLedRGB* led = GetSingleLedMock(x, y, z, matrix.leds);
                EXPECT_CALL(*led, enable()).Times(1);
            }
        }
    }

    MatrixOperation enable_all = EnableAll(XYZCoordinates(3, 3, 3));
    matrix.action(enable_all, LedSwitch::ENABLE);
}

TEST_F(LedMatrixTest, CanDisableAllLedsInArray) {
    LedMatrix matrix(4, 4, 4, *ledFactory);

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                MockLedRGB* led = GetSingleLedMock(x, y, z, matrix.leds);
                EXPECT_CALL(*led, disable()).Times(1);
            }
        }
    }

    MatrixOperation enable_all = EnableAll(XYZCoordinates(3, 3, 3));
    matrix.action(enable_all, LedSwitch::DISABLE);
}

TEST_F(LedMatrixTest, CanSetColorToSingleLed) {
    LedMatrix matrix(4, 4, 4, *ledFactory);

    MockLedRGB* led = GetSingleLedMock(2, 2, 2, matrix.leds);
    EXPECT_CALL(*led, setColor(Color::RED)).Times(1);
    EXPECT_CALL(*led, enable()).Times(1);

    MatrixOperation enable_single = EnableSingle(XYZCoordinates(2, 2, 2));
    matrix.action(enable_single, LedSwitch::ENABLE, Color::RED);
}

TEST_F(LedMatrixTest, CanDisableSingleLedInArray) {
    LedMatrix matrix(4, 4, 4, *ledFactory);

    MockLedRGB* led = GetSingleLedMock(1, 2, 3, matrix.leds);
    EXPECT_CALL(*led, disable()).Times(1);

    MatrixOperation enable_single = EnableSingle(XYZCoordinates(1, 2, 3));
    matrix.action(enable_single, LedSwitch::DISABLE);
}

TEST_F(LedMatrixTest, CanEnableSingleColumnAndSetColor) {
    LedMatrix matrix(3, 3, 3, *ledFactory);

    // Plane Z
    for (int z = 0; z < 3; z++) {
        // Get led mocks from column Z(1,2) <- plane Z, cartesian coordinates (x=1,y=2)
        MockLedRGB* led_z = GetSingleLedMock(1, 2, z, matrix.leds);
        EXPECT_CALL(*led_z, enable()).Times(1);
        EXPECT_CALL(*led_z, setColor(Color::CYAN)).Times(1);
    }

    MatrixOperation enable_Z_column = EnableColumn(ColumnCoordinates(Plane::Z, 1, 2, 3));
    matrix.action(enable_Z_column, LedSwitch::ENABLE, Color::CYAN);

    // Plane X
    for (int x = 0; x < 3; x++) {
        // Get led mocks from column X(0,2) <- plane X, cartesian coordinates (y=0,z=2)
        MockLedRGB* led_x = GetSingleLedMock(x, 0, 2, matrix.leds);
        EXPECT_CALL(*led_x, enable()).Times(1);
        EXPECT_CALL(*led_x, setColor(Color::MAGENTA)).Times(1);
    }

    MatrixOperation enable_X_column = EnableColumn(ColumnCoordinates(Plane::X, 0, 2, 3));
    matrix.action(enable_X_column, LedSwitch::ENABLE, Color::MAGENTA);

    // Plane Y
    for (int y = 0; y < 3; y++) {
        // Get led mocks from column Y(2,1) <- plane Y, cartesian coordinates (x=2,z=1)
        MockLedRGB* led_y = GetSingleLedMock(2, y, 1, matrix.leds);
        EXPECT_CALL(*led_y, enable()).Times(1);
        EXPECT_CALL(*led_y, setColor(Color::BLUE)).Times(1);
    }

    MatrixOperation enable_Y_column = EnableColumn(ColumnCoordinates(Plane::Y, 2, 1, 3));
    matrix.action(enable_Y_column, LedSwitch::ENABLE, Color::BLUE);
}

TEST_F(LedMatrixTest, CanEnableSingleZPlane) {
    LedMatrix matrix(4, 4, 4, *ledFactory);

    // Plane Z
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            // Get all led mocks from plane Z(3) <- third Z plane
            MockLedRGB* led = GetSingleLedMock(x, y, 2, matrix.leds);
            EXPECT_CALL(*led, enable()).Times(1);
        }
    }

    MatrixOperation enable_Z_plane = EnablePlane(PlaneCoordinates(Plane::Z, 2));
    matrix.action(enable_Z_plane, LedSwitch::ENABLE);
}

TEST_F(LedMatrixTest, CanEnableSingleXPlane) {
    LedMatrix matrix(4, 5, 6, *ledFactory);

    // Plane X
    for (int y = 0; y < 5; y++) {
        for (int z = 0; z < 6; z++) {
            // Get all led mocks from plane X(3) <- fourth X plane
            MockLedRGB* led = GetSingleLedMock(3, y, z, matrix.leds);
            EXPECT_CALL(*led, enable()).Times(1);
        }
    }

    MatrixOperation enable_X_plane = EnablePlane(PlaneCoordinates(Plane::X, 3));
    matrix.action(enable_X_plane, LedSwitch::ENABLE);
}

TEST_F(LedMatrixTest, CanEnableSingleYPlane) {
    LedMatrix matrix(6, 5, 4, *ledFactory);

    // Plane X
    for (int x = 0; x < 6; x++) {
        for (int z = 0; z < 4; z++) {
            // Get all led mocks from plane Y(4) <- fifth Y plane
            MockLedRGB* led = GetSingleLedMock(x, 4, z, matrix.leds);
            EXPECT_CALL(*led, enable()).Times(1);
        }
    }

    MatrixOperation enable_Y_plane = EnablePlane(PlaneCoordinates(Plane::Y, 4));
    matrix.action(enable_Y_plane, LedSwitch::ENABLE);
}

TEST_F(LedMatrixTest, CanEnableSingleCuboid) {
    LedMatrix matrix(4, 4, 4, *ledFactory);

    // Cuboid ((2, 2, 2),(3, 3, 3))
    for (int x = 2; x <= 3; x++) {
        for (int y = 2; y <= 3; y++) {
            for (int z = 2; z <= 3; z++) {
                MockLedRGB* led = GetSingleLedMock(x, y, z, matrix.leds);
                EXPECT_CALL(*led, enable()).Times(1);
            }
        }
    }

    XYZCoordinates S = XYZCoordinates(2, 2, 2);
    XYZCoordinates E = XYZCoordinates(3, 3, 3);
    MatrixOperation enable_cuboid = EnableCuboid(CuboidCoordinates(S, E));
    matrix.action(enable_cuboid, LedSwitch::ENABLE);
}
