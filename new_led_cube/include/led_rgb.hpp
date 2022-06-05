#include <iostream>

enum class Color : std::uint8_t {
    RED,
    GREEN,
    BLUE
};

class LedRGB {
public:
    LedRGB();
    ~LedRGB();
    void setColor(Color color_);
protected:
    Color color;
};
