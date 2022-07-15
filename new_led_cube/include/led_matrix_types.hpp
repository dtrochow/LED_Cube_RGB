enum class Dimension {
    X,
    Y,
    Z
};

enum class Plane {
    X,
    Y,
    Z
};

class Coordinates {
public:
    virtual ~Coordinates() {};
};

class CartesianCoordinates : public Coordinates {
public:
    CartesianCoordinates(int x_, int y_, int z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    int x;
    int y;
    int z;
};

class ColumnCoordinates : public Coordinates {
public:
    ColumnCoordinates(Plane plane_, int first_coordinate_, int second_coordinate_, int height_, int start_ = 0) {
        first_coordinate = first_coordinate_;
        second_coordinate = second_coordinate_;
        start = start_;
        height = height_;
        plane = plane_;
    }
    int first_coordinate;
    int second_coordinate;
    int start;
    int height;
    Plane plane;
};
