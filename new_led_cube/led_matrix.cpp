#include <map>
#include "led_matrix.hpp"

using namespace std;

EnableAll::EnableAll(LedRGB3DMatrix matrix) {
    led_matrix = matrix;
}

void EnableAll::run() {
    led_matrix[0]->enable();
    led_matrix[1]->enable();
    led_matrix[2]->enable();
    led_matrix[3]->enable();
    led_matrix[1]->enable();
    // led_matrix[0][0][0]->enable();
    // led_matrix[2][2][3]->enable();
    // led_matrix[3][3][3]->enable();
}

LedMatrix::LedMatrix(int x, int y, int z, const LedCreator& factory) {
    size_x = x;
    size_y = y;
    size_z = z;
    // leds.resize(size_x, vector<vector<LedRGB*>>(size_y, vector<LedRGB*>(size_z, factory.MakeLed())));
    leds.push_back(factory.MakeLed());
    leds.push_back(factory.MakeLed());
    leds.push_back(factory.MakeLed());
    leds.push_back(factory.MakeLed());
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

void LedMatrix::enableAll() {
    EnableAll ea(leds);
    ea.run();
}
