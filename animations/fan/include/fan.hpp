#include "animation_utils.hpp"

class Fan : public Animation {
public:
    Fan() {};
    ~Fan() override {};
public:
    void run(LedCube& cube, const AnimationSpeed speed, const int iterations) override;
private:
    cubeDim_t cube_dimensions;
    int64_t one_frame_time_us;

    void render_frame(LedCube& cube, const std::vector<XYCoordinates>& frame, int start, int height, Color color)  const;
    void render(LedCube& cube) const;
};
