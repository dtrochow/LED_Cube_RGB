#include "animation_utils.hpp"

class Fan : public Animation {
public:
    Fan() {};
    ~Fan() override {};
public:
    void run(LedCube& cube, const AnimationSpeed speed, const int iterations) override;
};
