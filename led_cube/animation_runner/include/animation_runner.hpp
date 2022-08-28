#include "animation_runner_types.hpp"
#include "led_cube.hpp"
#include "animation_utils.hpp"

class AnimationsRunner {
public:
    AnimationsRunner(LedCube* cube_);
    ~AnimationsRunner() {};
public:
    void run(AnimationType a_type, AnimationSpeed speed = AnimationSpeed::NORMAL, int iterations = 1);
private:
    LedCube* cube;
    std::map<AnimationType, Animation*> animations;
};
