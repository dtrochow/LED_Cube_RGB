#include <map>
#include "led_matrix.hpp"

namespace
{

void enableDisableLed(LedRGB3DMatrix &led_matrix, int x, int y, int z, LedSwitch switch_state) {
    if (LedSwitch::ENABLE == switch_state) {
        led_matrix[x][y][z]->enable();
    } else if (LedSwitch::DISABLE == switch_state) {
        led_matrix[x][y][z]->disable();
    }
}
void setLedColor(LedRGB3DMatrix &led_matrix, int x, int y, int z, Color color) {
    if (!(Color::NONE == color)) {
        led_matrix[x][y][z]->setColor(color);
    }
}
void setLedState(LedRGB3DMatrix &led_matrix, int x, int y, int z, LedSwitch switch_state, Color color) {
    enableDisableLed(led_matrix, x, y, z, switch_state);
    setLedColor(led_matrix, x, y, z, color);
}
void resizeMatrixVector(LedRGB3DMatrix &matrix, const int x_dim, const int y_dim, const int z_dim) {
    matrix.resize(x_dim);
    for (auto& vecX : matrix) {
        vecX.resize(y_dim);
        for (auto& vecY : vecX) {
            vecY.resize(z_dim);
        }
    }
}

}

LedMatrix::LedMatrix(int x, int y, int z, LedCreator &factory) {
    size.x = x;
    size.y = y;
    size.z = z;
    fillMatrixWithLeds(factory);
    enable_counter.resize(size.x, std::vector<std::vector<int>>(size.y, std::vector<int>(size.z, 0)));
}

void LedMatrix::fillMatrixWithLeds(LedCreator &factory) {
    resizeMatrixVector(leds, size.x, size.y, size.z);

    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            for (int z = 0; z < size.z; z++) {
                leds[x][y][z] = std::move(factory.MakeLed());
            }
        }
    }
}

int LedMatrix::getDimension(Dimension dim) {
    int dimension = 0;

    switch (dim) {
        case Dimension::X:
            dimension = size.x;
            break;
        case Dimension::Y:
            dimension = size.y;
            break;
        case Dimension::Z:
            dimension = size.z;
            break;
    }
    return dimension;
}

void LedMatrix::reset() {
    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            for (int z = 0; z < size.z; z++) {
                setLedState(leds, x, y, z, LedSwitch::DISABLE, Color::NONE);
            }
        }
    }
}

void LedMatrix::action(MatrixOperation const& operation, LedSwitch switch_state, Color color) {
    auto matrixOperationCall = [&](const auto& matrix_operation) {
        Run{}(matrix_operation, leds, size, switch_state, color);
    };
    std::visit(matrixOperationCall, operation);
}

/* -------------------------------------------------------------------------- */
/*                           Run method definitions                           */
/* -------------------------------------------------------------------------- */

void Run::operator()(EnableAll const& op, LedRGB3DMatrix &led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) const
{
    for (int x = 0; x <= op.coordinates.x; x++) {
        for (int y = 0; y <= op.coordinates.y; y++) {
            for (int z = 0; z <= op.coordinates.z; z++) {
                setLedState(led_matrix, x, y, z, switch_state, color);
            }
        }
    }
}

void Run::operator()(EnableSingle const& op, LedRGB3DMatrix &led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) const
{
    setLedState(led_matrix, op.coordinates.x, op.coordinates.y, op.coordinates.z, switch_state, color);
}

void Run::operator()(EnableColumn const& op, LedRGB3DMatrix &led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) const
{
    const int end = op.coordinates.start + op.coordinates.height;

    switch (op.coordinates.plane) {
        case Plane::X:
            for (int x = op.coordinates.start; x < end; x++) {
                setLedState(led_matrix, x, op.coordinates.first_coordinate,
                               op.coordinates.second_coordinate, switch_state, color);
            }
            break;
        case Plane::Y:
            for (int y = op.coordinates.start; y < end; y++) {
                setLedState(led_matrix, op.coordinates.first_coordinate, y,
                               op.coordinates.second_coordinate, switch_state, color);
            }
            break;
        case Plane::Z:
            for (int z = op.coordinates.start; z < end; z++) {
                setLedState(led_matrix, op.coordinates.first_coordinate,
                               op.coordinates.second_coordinate, z, switch_state, color);
            }
            break;
    }
}

void Run::operator()(EnablePlane const& op, LedRGB3DMatrix &led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) const
{
    switch (op.coordinates.plane) {
        case Plane::X:
            for (int y = 0; y < size.y; y++) {
                for (int z = 0; z < size.z; z++) {
                    setLedState(led_matrix, op.coordinates.position,
                                   y, z, switch_state, color);
                }
            }
            break;
        case Plane::Y:
            for (int x = 0; x < size.x; x++) {
                for (int z = 0; z < size.z; z++) {
                    setLedState(led_matrix, x, op.coordinates.position,
                                   z, switch_state, color);
                }
            }
            break;
        case Plane::Z:
            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    setLedState(led_matrix, x, y,
                                   op.coordinates.position, switch_state, color);
                }
            }
            break;
    }
}

void Run::operator()(EnableCuboid const& op, LedRGB3DMatrix &led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) const
{
    for (int x = op.coordinates.start.x; x <= op.coordinates.end.x; x++) {
        for (int y = op.coordinates.start.y; y <= op.coordinates.end.y; y++) {
            for (int z = op.coordinates.start.z; z <= op.coordinates.end.z; z++) {
                setLedState(led_matrix, x, y, z, switch_state, color);
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
