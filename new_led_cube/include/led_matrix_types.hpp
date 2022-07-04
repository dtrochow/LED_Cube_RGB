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

class PlaneCoordinates : public Coordinates {
    PlaneCoordinates(Plane plane_, int position_) {
        position = position_;
        plane = plane_;
    }
    int position;
    Plane plane;
};
