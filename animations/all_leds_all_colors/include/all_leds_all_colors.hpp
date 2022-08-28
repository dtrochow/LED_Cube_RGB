#include "animation_utils.hpp"

class AllLedsAllColors : public Animation {
public:
    AllLedsAllColors() {};
    ~AllLedsAllColors() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed, int iterations) override;
};
