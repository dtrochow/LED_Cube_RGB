#pragma once

#include "animations_types.hpp"
#include "led_cube.hpp"
#include <map>

class Animation {
public:
    virtual ~Animation() {};
public:
    virtual void run(LedCube* cube, AnimationSpeed speed) = 0;
};

class AnimationsRunner {
public:
    AnimationsRunner(LedCube* cube_);
    ~AnimationsRunner() {};
public:
    void run(AnimationType a_type, AnimationSpeed speed = AnimationSpeed::NORMAL);
private:
    LedCube* cube;
    std::map<AnimationType, Animation*> animations;
};

class AllLedsAllColors : public Animation {
public:
    AllLedsAllColors() {};
    ~AllLedsAllColors() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed) override;
};
