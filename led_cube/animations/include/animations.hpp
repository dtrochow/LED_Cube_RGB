#pragma once

#include "animations_types.hpp"
#include "led_cube.hpp"
#include <map>

class Animation {
public:
    virtual ~Animation() {};
public:
    /**
     * @param cube - pointer to a LedCube object
     * @param speed  - one of 5 available speed options for animation
     * @param iterations - number of animation iterations
     */
    virtual void run(LedCube* cube, AnimationSpeed speed, int iterations) = 0;
};

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

class AllLedsAllColors : public Animation {
public:
    AllLedsAllColors() {};
    ~AllLedsAllColors() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed, int iterations) override;
};

class RandomCubeAndColor : public Animation {
public:
    RandomCubeAndColor() {};
    ~RandomCubeAndColor() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed, int iterations) override;
};

class Rain : public Animation {
public:
    Rain() {};
    ~Rain() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed, int iterations) override;
};

class ChessCubes : public Animation {
public:
    ChessCubes() {};
    ~ChessCubes() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed, int iterations) override;
};
