#include "animation_utils.hpp"

class RandomCubeAndColor : public Animation {
public:
    RandomCubeAndColor() {};
public:
    void run(LedCube& cube, const AnimationSpeed speed, const int iterations) override;
};
