#pragma once

#include "led_matrix_types.hpp"
#include "led_rgb.hpp"

#include <vector>
#include <map>
using namespace std;

typedef vector<vector<vector<LedRGB*>>> LedRGB3DMatrix;
typedef vector<vector<vector<int>>> Int3DMatrix;

class MatrixOperation {
public:
    virtual ~MatrixOperation() {};
    virtual void run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color = Color::NONE) {};
};

class EnableAll : public MatrixOperation {
public:
    EnableAll(CartesianCoordinates* coordinates_) {
        coordinates = coordinates_;
    };
    void run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color = Color::NONE) override;
    CartesianCoordinates* coordinates;
};

class EnableSingle : public MatrixOperation {
public:
    EnableSingle(CartesianCoordinates* coordinates_) {
        coordinates = coordinates_;
    }
    void run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color = Color::NONE) override;
    CartesianCoordinates* coordinates;
};

class EnableColumn: public MatrixOperation {
public:
    EnableColumn(ColumnCoordinates* coordinates_) {
        coordinates = coordinates_;
    }
    void run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color = Color::NONE) override;
    ColumnCoordinates* coordinates;
};

class EnablePlane: public MatrixOperation {
public:
    EnablePlane(PlaneCoordinates* coordinates_) {
        coordinates = coordinates_;
    };
    void run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color = Color::NONE) override;
    PlaneCoordinates* coordinates;
};

class EnableCuboid: public MatrixOperation {
public:
    EnableCuboid(CuboidCoordinates* coordinates_) {
        coordinates = coordinates_;
    }
    void run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color = Color::NONE) override;
    CuboidCoordinates* coordinates;
};

class LedMatrix {
public:
    LedMatrix(int x, int y, int z, const LedCreator& factory);
    ~LedMatrix() {};
    int getDimension(Dimension dim);
    LedRGB3DMatrix leds;
    void action(MatrixOperation* operation, LedSwitch switch_state, Color color = Color::NONE);
    void enableAll();
protected:
    matrixSize_t size;
    Int3DMatrix enable_counter;
private:
    void fillMatrixWithLeds(const LedCreator& factory);
};
