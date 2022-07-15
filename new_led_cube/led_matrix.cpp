#include <map>
#include "led_matrix.hpp"

using namespace std;

LedMatrix::LedMatrix(int x, int y, int z, const LedCreator& factory) {
    size_x = x;
    size_y = y;
    size_z = z;
    fillMatrixWithLeds(factory);
    enable_counter.resize(size_x, vector<vector<int>>(size_y, vector<int>(size_z, 0)));
}

void LedMatrix::fillMatrixWithLeds(const LedCreator& factory) {
    for (int i = 0; i < size_x; i++) {
        vector<vector<LedRGB*>> v_y;
        for (int j = 0; j < size_y; j++) {
            vector<LedRGB*> v_z;
            for (int k = 0; k < size_z; k ++) {
                v_z.push_back(factory.MakeLed());
            }
            v_y.push_back(v_z);
        }
        leds.push_back(v_y);
    }
}

int LedMatrix::getDimension(Dimension dim) {
    switch(dim) {
        case Dimension::X:
            return size_x;
        case Dimension::Y:
            return size_y;
        case Dimension::Z:
            return size_z;
    }
}

void LedMatrix::action(MatrixOperation* operation, LedSwitch switch_state, Color color) {
    operation->run(leds, switch_state, color);
}

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

void EnableAll::run(LedRGB3DMatrix led_matrix, LedSwitch switch_state, Color color) {
    for (int x = 0; x < coordinates->x; x++) {
        for (int y= 0; y < coordinates->y; y++) {
            for (int z = 0; z < coordinates->z; z ++) {
                LM_EnableDisableLed(led_matrix, x, y, z, switch_state);
                LM_SetLedColor(led_matrix, x, y, z, color);
            }
        }
    }
}

void EnableSingle::run(LedRGB3DMatrix led_matrix, LedSwitch switch_state, Color color) {
    LM_EnableDisableLed(led_matrix, coordinates->x, coordinates->y, coordinates->z, switch_state);
    LM_SetLedColor(led_matrix, coordinates->x, coordinates->y, coordinates->z, color);
}
