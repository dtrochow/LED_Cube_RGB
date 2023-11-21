#include "animation_utils.hpp"

class RaisingColumns : public Animation {
public:
    RaisingColumns() {};
public:
    void run(LedCube& cube, const AnimationSpeed speed, const int iterations) override;
};
