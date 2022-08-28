#include "animation_utils.hpp"

class RaisingColumns : public Animation {
public:
    RaisingColumns() {};
    ~RaisingColumns() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed, int iterations) override;
};
