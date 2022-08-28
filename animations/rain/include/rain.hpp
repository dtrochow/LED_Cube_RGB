#include "animation_utils.hpp"

class Rain : public Animation {
public:
    Rain() {};
    ~Rain() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed, int iterations) override;
};
