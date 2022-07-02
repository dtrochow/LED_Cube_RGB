#include <map>
#include "led_matrix.hpp"

LedMatrix::LedMatrix(int x, int y, int z ,const LedCreator& factory) {
    size_x = x;
    size_y = y;
    size_z = z;
    leds.resize(size_x, vector<vector<LedRGB*>>(size_y, vector<LedRGB*>(size_z, factory.MakeLed())));
    enable_counter.resize(size_x, vector<vector<int>>(size_y, vector<int>(size_z, 0)));
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

LedMatrix::~LedMatrix() {}
