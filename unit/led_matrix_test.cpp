#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "led_matrix.hpp"
#include "led_rgb.hpp"
#include "mocks/led_rgb_mock.h"

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

// [X] 1. Make method for filling all matrix fields with LedRGB objects
// [X] 2. Make helper method for destroying mock objects
// [X] 3. Enable All
// [ ] 4. Enable column
// [ ] 5. Enaple row
// [ ] 6. Enable plane
// [ ] 7. Enable single led

TEST_F(LedMatrixTest, CanEnableAllLedsInArray) {
    LedMatrix* matrix = new LedMatrix(4, 4, 4, *ledFactory);
    mock_leds.push_back(GetSingleLedMock(0, 0, 0, matrix->leds));
    mock_leds.push_back(GetSingleLedMock(3, 3, 3, matrix->leds));
    mock_leds.push_back(GetSingleLedMock(1, 2, 3, matrix->leds));
    mock_leds.push_back(GetSingleLedMock(3, 3, 0, matrix->leds));
    EXPECT_CALL(*mock_leds[0], enable()).Times(1);
    EXPECT_CALL(*mock_leds[1], enable()).Times(1);
    EXPECT_CALL(*mock_leds[2], enable()).Times(1);
    EXPECT_CALL(*mock_leds[3], enable()).Times(1);
    matrix->action(Action::ENABLE_ALL);
}

TEST_F(LedMatrixTest, CanSetTheMatrixSize) {
    LedMatrix lMatrix(4, 5, 6, *ledFactory);
    EXPECT_EQ(lMatrix.getDimension(Dimension::X), 4);
    EXPECT_EQ(lMatrix.getDimension(Dimension::Y), 5);
    EXPECT_EQ(lMatrix.getDimension(Dimension::Z), 6);
}
