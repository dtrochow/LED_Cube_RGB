#include "animation_runner.hpp"
#include "animations.hpp"

/* -------------------------------------------------------------------------- */
/*                              Animations Runner                             */
/* -------------------------------------------------------------------------- */

AnimationsRunner::AnimationsRunner(LedCube* cube_) {
    cube = cube_;
    animations = {
		{ AnimationType::SNAKE, new Snake() },
		{ AnimationType::RAISING_COLUMNS, new RaisingColumns() },
		{ AnimationType::RAIN, new Rain() },
		{ AnimationType::RANDOM_CUBE_AND_COLOR, new RandomCubeAndColor() },
		{ AnimationType::ALL_LEDS_ALL_COLORS, new AllLedsAllColors() },
        { AnimationType::SNAKE, new Snake() }
    };
}

void AnimationsRunner::run(AnimationType a_type, AnimationSpeed speed, int iterations) {
    animations[a_type]->run(cube, speed, iterations);
}
