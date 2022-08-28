#include "animation_utils.hpp"

class RandomCubeAndColor : public Animation {
public:
    RandomCubeAndColor() {};
    ~RandomCubeAndColor() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed, int iterations) override;
};
