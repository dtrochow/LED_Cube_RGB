#include "led_matrix_types.hpp"
#include "led_rgb.hpp"

#include <vector>
#include <map>
using namespace std;

typedef vector<vector<vector<LedRGB*>>> LedRGB3DMatrix;
typedef vector<vector<vector<int>>> Int3DMatrix;

enum class Action {
    ENABLE_ALL,
    ENABLE_SINGLE
};

class MatrixOperation {
public:
    virtual ~MatrixOperation() {}
    virtual void run(LedRGB3DMatrix matrix) {} 
};

class EnableAll : public MatrixOperation {
public:
    EnableAll(CartesianCoordinates* coordinates_) {
        coordinates = coordinates_;
    };
    void run(LedRGB3DMatrix matrix) override;
    CartesianCoordinates* coordinates;
};

class EnableSingle : public MatrixOperation {
public:
    EnableSingle(CartesianCoordinates* coordinates_) {
        coordinates = coordinates_;
    }
    void run(LedRGB3DMatrix matrix) override;
    CartesianCoordinates* coordinates;
};

class LedMatrix {
public:
    LedMatrix(int x, int y, int z, const LedCreator& factory);
    ~LedMatrix() {};
    int getDimension(Dimension dim);
    LedRGB3DMatrix leds;
    void action(MatrixOperation* operation);
    void enableAll();
protected:
    int size_x;
    int size_y;
    int size_z;
    Int3DMatrix enable_counter;
private:
    void fillMatrixWithLeds(const LedCreator& factory);
};


