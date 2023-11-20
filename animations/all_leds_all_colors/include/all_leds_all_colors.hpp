#include "animation_utils.hpp"

class AllLedsAllColors : public Animation {
public:
    AllLedsAllColors() {};
public:
    void run(LedCube& cube, const AnimationSpeed speed, const int iterations) override;
};
