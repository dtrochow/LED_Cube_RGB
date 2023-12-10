#include <memory>

#include "animation_runner.hpp"
#include "animations.hpp"

/* -------------------------------------------------------------------------- */
/*                              Animations Runner                             */
/* -------------------------------------------------------------------------- */

AnimationsRunner::AnimationsRunner(std::unique_ptr<LedCube> cube_)
{
    cube = std::move(cube_);
    animations = {
        { AnimationType::FAN,                   new Fan() },
        { AnimationType::SNAKE,                 new Snake() },
        { AnimationType::RAISING_COLUMNS,       new RaisingColumns() },
        { AnimationType::RAIN,                  new Rain() },
        { AnimationType::RANDOM_CUBE_AND_COLOR, new RandomCubeAndColor() },
        { AnimationType::ALL_LEDS_ALL_COLORS,   new AllLedsAllColors() },
    };
}

void AnimationsRunner::run(AnimationType a_type, AnimationSpeed speed, int iterations)
{
    animations[a_type]->run(*cube, speed, iterations);
}
