#include <map>
#include "led_matrix.hpp"

using namespace std;

class EnableAll : public MatrixOperation {
public:
    EnableAll(LedRGB3DMatrix matrix) {
        led_matrix = matrix;
    }
    void run() override {
        for (int i = 0; i < led_matrix.size(); i++) {
            for (int j = 0; j < led_matrix[0].size(); j++) {
                for (int k = 0; k < led_matrix[0][0].size(); k ++) {
                    led_matrix[i][j][k]->enable();
        }}}}
private:
    LedRGB3DMatrix led_matrix;
};

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

void LedMatrix::action(Action a) {
    MatrixOperation* operation;
    switch(a) {
        case Action::ENABLE_ALL:
            operation = new EnableAll(leds);
            break;
        // case Action::ENABLE_SINGLE:
        //     operation = new EnableSingle(leds);
        //     break;
    }
    operation->run();
}
