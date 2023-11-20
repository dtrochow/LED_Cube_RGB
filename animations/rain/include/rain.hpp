#include "animation_utils.hpp"

class Rain : public Animation {
public:
    Rain() {};
public:
    void run(LedCube& cube, const AnimationSpeed speed, const int iterations) override;
};
