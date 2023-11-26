enum class Dimension : int {
    X,
    Y,
    Z
};

enum class Plane : int {
    X,
    Y,
    Z
};

typedef struct {
    int x;
    int y;
    int z;
} matrixSize_t;

class Coordinates {
public:
    virtual ~Coordinates() = default;
};

class CartesianCoordinates : public Coordinates {
public:
    CartesianCoordinates(int x_, int y_, int z_) :
        x(x_), y(y_), z(z_) {};
    int x;
    int y;
    int z;

    // Implement the less-than operator for comparison
    bool operator<(const CartesianCoordinates& other) const
    {
        if (x < other.x) return true;
        if (x > other.x) return false;
        if (y < other.y) return true;
        if (y > other.y) return false;
        return z < other.z;
    }

};

class ColumnCoordinates : public Coordinates {
public:
    ColumnCoordinates(Plane plane_, int first_coordinate_, int second_coordinate_, int height_, int start_ = 0) :
        first_coordinate(first_coordinate_), second_coordinate(second_coordinate_), start(start_), height(height_), plane(plane_) {};
    int first_coordinate;
    int second_coordinate;
    int start;
    int height;
    Plane plane;
};

class PlaneCoordinates : public Coordinates {
public:
    PlaneCoordinates(Plane plane_, int position_ = 0) :
        position(position_), plane(plane_) {};
    int position;
    Plane plane;
};

class CuboidCoordinates : public Coordinates {
public:
    CuboidCoordinates(CartesianCoordinates& cr_start_, CartesianCoordinates& cr_end_) :
        start(cr_start_), end(cr_end_){};
    CartesianCoordinates start;
    CartesianCoordinates end;
};
