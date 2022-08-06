#include <map>
#include "led_matrix.hpp"

using namespace std;

static void LM_EnableDisableLed(LedRGB3DMatrix led_matrix, int x, int y, int z, LedSwitch switch_state) {
    if (LedSwitch::ENABLE == switch_state) {
        led_matrix[x][y][z]->enable();
    }
    else if (LedSwitch::DISABLE == switch_state) {
        led_matrix[x][y][z]->disable();
    }
}

static void LM_SetLedColor(LedRGB3DMatrix led_matrix, int x, int y, int z, Color color) {
    if (!(Color::NONE == color)) {
        led_matrix[x][y][z]->setColor(color);
    }
}

static void LM_SetLedState(LedRGB3DMatrix led_matrix, int x, int y, int z, LedSwitch switch_state, Color color) {
    LM_EnableDisableLed(led_matrix, x, y, z, switch_state);
    LM_SetLedColor(led_matrix, x, y, z, color);
}

LedMatrix::LedMatrix(int x, int y, int z, LedCreator* factory) {
    size.x = x;
    size.y = y;
    size.z = z;
    fillMatrixWithLeds(factory);
    enable_counter.resize(size.x, vector<vector<int>>(size.y, vector<int>(size.z, 0)));
}

void LedMatrix::fillMatrixWithLeds(LedCreator* factory) {
    for (int i = 0; i < size.x; i++) {
        vector<vector<LedRGB*>> v_y;
        for (int j = 0; j < size.y; j++) {
            vector<LedRGB*> v_z;
            for (int k = 0; k < size.z; k ++) {
                v_z.push_back(factory->MakeLed());
            }
            v_y.push_back(v_z);
        }
        leds.push_back(v_y);
    }
}

int LedMatrix::getDimension(Dimension dim) {
    int dimension = 0;
    switch(dim) {
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
    for (int x = 0; x < size.x; x ++) {
        for (int y = 0; y < size.y; y ++) {
            for (int z = 0; z < size.z; z ++) {
                LM_SetLedState(leds, x, y, z, LedSwitch::DISABLE, Color::NONE);
            }
        }
    }
}

void LedMatrix::action(MatrixOperation* operation, LedSwitch switch_state, Color color) {
    operation->run(leds, size, switch_state, color);
}

void EnableAll::run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) {
    for (int x = 0; x < coordinates->x; x++) {
        for (int y= 0; y < coordinates->y; y++) {
            for (int z = 0; z < coordinates->z; z ++) {
                LM_SetLedState(led_matrix, x, y, z, switch_state, color);
            }
        }
    }
}

void EnableSingle::run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) {
    LM_SetLedState(led_matrix, coordinates->x, coordinates->y, coordinates->z, switch_state, color);
}

void EnableColumn::run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) {
    int end = coordinates->start + coordinates->height;
    switch(coordinates->plane) {
        case Plane::X:
            for (int x = coordinates->start; x < end; x ++) {
                LM_SetLedState(led_matrix, x, coordinates->first_coordinate, 
                               coordinates->second_coordinate, switch_state, color);            }
            break;
        case Plane::Y:
            for (int y = coordinates->start; y < end; y ++) {
                LM_SetLedState(led_matrix, coordinates->first_coordinate, y,
                               coordinates->second_coordinate, switch_state, color);
            }
            break;
        case Plane::Z:
            for (int z = coordinates->start; z < end; z ++) {
                LM_SetLedState(led_matrix, coordinates->first_coordinate, 
                               coordinates->second_coordinate, z, switch_state, color);
            }
            break;
    }
}

void EnablePlane::run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) {
    switch(coordinates->plane) {
        case Plane::X:
            for (int y = 0; y < size.y; y ++) {
                for (int z = 0; z < size.z; z ++) {
                    LM_SetLedState(led_matrix, coordinates->position, 
                                   y, z, switch_state, color);
                }
            }
            break;
        case Plane::Y:
            for (int x = 0; x < size.x; x ++) {
                for (int z = 0; z < size.z; z ++) {
                    LM_SetLedState(led_matrix, x, coordinates->position, 
                                   z, switch_state, color);
                }
            }
            break;
        case Plane::Z:
            for (int x = 0; x < size.x; x ++) {
                for (int y = 0; y < size.y; y ++) {
                    LM_SetLedState(led_matrix, x, y,
                                   coordinates->position, switch_state, color);
                }
            }
            break;
    }
}

void EnableCuboid::run(LedRGB3DMatrix led_matrix, matrixSize_t size, LedSwitch switch_state, Color color) {
    for (int x = coordinates->start->x; x < coordinates->end->x; x++) {
        for (int y= coordinates->start->y; y < coordinates->end->y; y++) {
            for (int z = coordinates->start->z; z < coordinates->end->z; z ++) {
                LM_SetLedState(led_matrix, x, y, z, switch_state, color);
            }
        }
    }
}
