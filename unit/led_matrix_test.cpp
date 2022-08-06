#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "led_matrix.hpp"
#include "led_rgb.hpp"
#include "led_rgb_mock.h"

using ::testing::Return;

template<class T> class DeleteVector
{
    public:
    bool operator()(T x) const
    {
        delete x;
        return true;
    }
};

class MockLedCreator : public LedCreator {
public:
    MockLedCreator() {};
    LedRGB* CreateMethod() const override {
        return new MockLedRGB;
    }
};

class LedMatrixTest : public ::testing::Test {
public:
    void SetUp() override {

    }
    void TearDown() override {
        for_each(mock_leds.begin(), mock_leds.end(), DeleteVector<MockLedRGB*>());
    }
    LedMatrixTest() {};
    MockLedRGB* GetSingleLedMock(int x, int y, int z, LedRGB3DMatrix matrix) {
        return (MockLedRGB*)matrix[x][y][z];
    }
protected:
    LedCreator* ledFactory = new MockLedCreator();
    vector<MockLedRGB*> mock_leds;
};

TEST_F(LedMatrixTest, CanSetTheMatrixSize) {
    LedMatrix lMatrix(4, 5, 6, ledFactory);
    EXPECT_EQ(lMatrix.getDimension(Dimension::X), 4);
    EXPECT_EQ(lMatrix.getDimension(Dimension::Y), 5);
    EXPECT_EQ(lMatrix.getDimension(Dimension::Z), 6);
}

TEST_F(LedMatrixTest, CanEnableAllLedsInArray) {
    LedMatrix* matrix = new LedMatrix(4, 4, 4, ledFactory);
    for (int i = 0; i < 4; i ++) {
        for (int j = 0; j < 4; j ++) {
            for (int k = 0; k < 4; k ++) {
                mock_leds.push_back(GetSingleLedMock(i, j, k, matrix->leds));
                EXPECT_CALL(*mock_leds.back(), enable()).Times(1);
            }
        }
    }
    CartesianCoordinates* cr = new CartesianCoordinates(3, 3, 3);
    MatrixOperation* enable_all = new EnableAll(cr);
    matrix->action(enable_all, LedSwitch::ENABLE);
}

TEST_F(LedMatrixTest, CanDisableAllLedsInArray) {
    LedMatrix* matrix = new LedMatrix(4, 4, 4, ledFactory);
    for (int i = 0; i < 4; i ++) {
        for (int j = 0; j < 4; j ++) {
            for (int k = 0; k < 4; k ++) {
                mock_leds.push_back(GetSingleLedMock(i, j, k, matrix->leds));
                EXPECT_CALL(*mock_leds.back(), disable()).Times(1);
            }
        }
    }
    CartesianCoordinates* cr = new CartesianCoordinates(3, 3, 3);
    MatrixOperation* enable_all = new EnableAll(cr);
    matrix->action(enable_all, LedSwitch::DISABLE);
}

TEST_F(LedMatrixTest, CanSetColorToSingleLed) {
    LedMatrix* matrix = new LedMatrix(4, 4, 4, ledFactory);
    mock_leds.push_back(GetSingleLedMock(2, 2, 2, matrix->leds));
    EXPECT_CALL(*mock_leds[0], setColor(Color::RED)).Times(1);
    EXPECT_CALL(*mock_leds[0], enable()).Times(1);
    CartesianCoordinates* cr = new CartesianCoordinates(2, 2, 2);
    MatrixOperation* enable_single = new EnableSingle(cr);
    matrix->action(enable_single, LedSwitch::ENABLE, Color::RED);
}

TEST_F(LedMatrixTest, CanDisableSingleLedInArray) {
    LedMatrix* matrix = new LedMatrix(4, 4, 4, ledFactory);
    mock_leds.push_back(GetSingleLedMock(1, 2, 3, matrix->leds));
    EXPECT_CALL(*mock_leds[0], disable()).Times(1);
    CartesianCoordinates* cr = new CartesianCoordinates(1, 2, 3);
    MatrixOperation* enable_single = new EnableSingle(cr);
    matrix->action(enable_single, LedSwitch::DISABLE);
}

TEST_F(LedMatrixTest, CanEnableSingleColumnAndSetColor) {
    LedMatrix* matrix = new LedMatrix(3, 3, 3, ledFactory);
    // Plane Z
    for (int i = 0; i < 3; i ++) {
        // Get led mocks from column Z(1,2) <- plane Z, cartesian coordinates (x=1,y=2)
        mock_leds.push_back(GetSingleLedMock(1, 2, i, matrix->leds));
        EXPECT_CALL(*mock_leds.back(), enable()).Times(1);
        EXPECT_CALL(*mock_leds.back(), setColor(Color::CYAN)).Times(1);
    }
    ColumnCoordinates* cr_z = new ColumnCoordinates(Plane::Z, 1, 2, 3);
    MatrixOperation* enable_Z_column = new EnableColumn(cr_z);
    matrix->action(enable_Z_column, LedSwitch::ENABLE, Color::CYAN);
    // Plane X
    for (int i = 0; i < 3; i ++) {
        // Get led mocks from column X(0,2) <- plane X, cartesian coordinates (y=0,z=2)
        mock_leds.push_back(GetSingleLedMock(i, 0, 2, matrix->leds));
        EXPECT_CALL(*mock_leds.back(), enable()).Times(1);
        EXPECT_CALL(*mock_leds.back(), setColor(Color::MAGENTA)).Times(1);
    }
    ColumnCoordinates* cr_x = new ColumnCoordinates(Plane::X, 0, 2, 3);
    MatrixOperation* enable_X_column = new EnableColumn(cr_x);
    matrix->action(enable_X_column, LedSwitch::ENABLE, Color::MAGENTA);
    // Plane Y
    for (int i = 0; i < 3; i ++) {
        // Get led mocks from column Y(2,1) <- plane Y, cartesian coordinates (x=2,z=1)
        mock_leds.push_back(GetSingleLedMock(2, i, 1, matrix->leds));
        EXPECT_CALL(*mock_leds.back(), enable()).Times(1);
        EXPECT_CALL(*mock_leds.back(), setColor(Color::BLUE)).Times(1);
    }
    ColumnCoordinates* cr_y = new ColumnCoordinates(Plane::Y, 2, 1, 3);
    MatrixOperation* enable_Y_column = new EnableColumn(cr_y);
    matrix->action(enable_Y_column, LedSwitch::ENABLE, Color::BLUE);
}

TEST_F(LedMatrixTest, CanEnableSingleZPlane) {
    LedMatrix* matrix = new LedMatrix(4, 4, 4, ledFactory);
    // Plane Z
    for (int x = 0; x < 4; x ++) {
        for (int y = 0; y < 4; y ++) {
            // Get all led mocks from plane Z(3) <- third Z plane
            mock_leds.push_back(GetSingleLedMock(x, y, 2, matrix->leds));
            EXPECT_CALL(*mock_leds.back(), enable()).Times(1);
        }
    }
    PlaneCoordinates* cr_z = new PlaneCoordinates(Plane::Z, 2);
    MatrixOperation* enable_Z_plane = new EnablePlane(cr_z);
    matrix->action(enable_Z_plane, LedSwitch::ENABLE);
}

TEST_F(LedMatrixTest, CanEnableSingleXPlane) {
    LedMatrix* matrix = new LedMatrix(4, 5, 6, ledFactory);
    // Plane X
    for (int y = 0; y < 5; y ++) {
        for (int z = 0; z < 6; z ++) {
            // Get all led mocks from plane X(3) <- fourth X plane
            mock_leds.push_back(GetSingleLedMock(3, y, z, matrix->leds));
            EXPECT_CALL(*mock_leds.back(), enable()).Times(1);
        }
    }
    PlaneCoordinates* cr_x = new PlaneCoordinates(Plane::X, 3);
    MatrixOperation* enable_X_plane = new EnablePlane(cr_x);
    matrix->action(enable_X_plane, LedSwitch::ENABLE);
}

TEST_F(LedMatrixTest, CanEnableSingleYPlane) {
    LedMatrix* matrix = new LedMatrix(6, 5, 4, ledFactory);
    // Plane X
    for (int x = 0; x < 6; x ++) {
        for (int z = 0; z < 4; z ++) {
            // Get all led mocks from plane Y(4) <- fifth Y plane
            mock_leds.push_back(GetSingleLedMock(x, 4, z, matrix->leds));
            EXPECT_CALL(*mock_leds.back(), enable()).Times(1);
        }
    }
    PlaneCoordinates* cr_y = new PlaneCoordinates(Plane::Y, 4);
    MatrixOperation* enable_Y_plane = new EnablePlane(cr_y);
    matrix->action(enable_Y_plane, LedSwitch::ENABLE);
}

TEST_F(LedMatrixTest, CanEnableSingleCuboid) {
    LedMatrix* matrix = new LedMatrix(4, 4, 4, ledFactory);
    // Cuboid ((2, 2, 2),(3, 3, 3))
    for (int x = 2; x <= 3; x ++) {
        for (int y = 2; y <= 3; y ++) {
            for (int z = 2; z <= 3; z ++) 
            mock_leds.push_back(GetSingleLedMock(x, y, z, matrix->leds));
            EXPECT_CALL(*mock_leds.back(), enable()).Times(1);
        }
    }
    CartesianCoordinates* cr_start = new CartesianCoordinates(2, 2, 2);
    CartesianCoordinates* cr_end = new CartesianCoordinates(3, 3, 3);
    CuboidCoordinates* cr = new CuboidCoordinates(cr_start, cr_end);
    MatrixOperation* enable_cuboid = new EnableCuboid(cr);
    matrix->action(enable_cuboid, LedSwitch::ENABLE);
}
