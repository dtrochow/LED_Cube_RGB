#include <memory>

#include "animation_runner_types.hpp"
#include "led_cube.hpp"
#include "animation_utils.hpp"

class AnimationsRunner {
public:
    AnimationsRunner(std::unique_ptr<LedCube> cube_);
    ~AnimationsRunner() {
        for (auto &animation: animations) {
            delete animation.second;
        }
    };
    void run(AnimationType a_type, AnimationSpeed speed = AnimationSpeed::NORMAL, int iterations = 1);
private:
    std::unique_ptr<LedCube> cube;
    std::map<AnimationType, Animation*> animations;
};
