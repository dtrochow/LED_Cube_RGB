#include "led_matrix_types.hpp"
#include "led_rgb.hpp"

#include <vector>
#include <map>
using namespace std;

class LedMatrix {
public:
    LedMatrix(int x, int y, int z, const LedCreator& factory);
    ~LedMatrix();
    int getDimension(Dimension dim);
    vector<vector<vector<LedRGB*>>> leds;
protected:
    int size_x;
    int size_y;
    int size_z;
    vector<vector<vector<int>>> enable_counter;
};
