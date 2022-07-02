#include "led_matrix_types.hpp"
#include "led_rgb.hpp"

#include <vector>
#include <map>
using namespace std;

typedef vector<LedRGB*> LedRGB3DMatrix;
typedef vector<vector<vector<int>>> Int3DMatrix;

class LedMatrix {
public:
    LedMatrix(int x, int y, int z, const LedCreator& factory);
    ~LedMatrix() {};
    int getDimension(Dimension dim);
    LedRGB3DMatrix leds;
    void enableAll();
protected:
    int size_x;
    int size_y;
    int size_z;
    Int3DMatrix enable_counter;
};

class MatrixOperation {
public:
    virtual ~MatrixOperation() {}
    virtual void run() {} 
};

class EnableAll : public MatrixOperation {
public:
    EnableAll(LedRGB3DMatrix matrix);
    void run() override;
private:
    LedRGB3DMatrix led_matrix;
};
