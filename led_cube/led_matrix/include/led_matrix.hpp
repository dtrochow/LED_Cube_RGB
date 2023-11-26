#pragma once

#include "led_matrix_types.hpp"
#include "led_rgb.hpp"

#include <vector>
#include <map>
#include <variant>

typedef std::vector<std::vector<std::vector<std::unique_ptr<LedRGB> > > >   LedRGB3DMatrix;
typedef std::vector<std::vector<std::vector<int> > >                       Int3DMatrix;

class EnableAll {
public:
    EnableAll(const CartesianCoordinates& coordinates_) :
        coordinates(coordinates_) {};
    CartesianCoordinates coordinates;
};

class EnableSingle {
public:
    EnableSingle(const CartesianCoordinates& coordinates_) :
        coordinates(coordinates_) {};
    CartesianCoordinates coordinates;
};

class EnableColumn {
public:
    EnableColumn(const ColumnCoordinates& coordinates_) :
        coordinates(coordinates_) {};
    ColumnCoordinates coordinates;
};

class EnablePlane {
public:
    EnablePlane(const PlaneCoordinates& coordinates_) :
        coordinates(coordinates_) {};
    PlaneCoordinates coordinates;
};

class EnableCuboid {
public:
    EnableCuboid(const CuboidCoordinates& coordinates_) :
        coordinates(coordinates_) {};
    CuboidCoordinates coordinates;
};

class Run {
public:
    void operator()(EnableAll const& op, LedRGB3DMatrix &led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) const;
    void operator()(EnableSingle const& op, LedRGB3DMatrix &led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) const;
    void operator()(EnableColumn const& op, LedRGB3DMatrix &led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) const;
    void operator()(EnablePlane const& op, LedRGB3DMatrix &led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) const;
    void operator()(EnableCuboid const& op, LedRGB3DMatrix &led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) const;
};

using MatrixOperation = std::variant<EnableAll, EnableSingle, EnableColumn, EnablePlane, EnableCuboid>;

class LedMatrix {
public:
    LedRGB3DMatrix leds;

    LedMatrix(int x, int y, int z, LedCreator& factory);
    ~LedMatrix() = default;
    int getDimension(Dimension dim);
    void action(MatrixOperation const& operation, LedSwitch switch_state, Color color = Color::NONE);
    void reset();
private:
    matrixSize_t size;
    Int3DMatrix enable_counter;
    void fillMatrixWithLeds(LedCreator &factory);
};
