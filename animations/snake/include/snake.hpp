#include "animation_utils.hpp"

class Snake : public Animation {
public:
    Snake() {};
    ~Snake() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed, int iterations) override;
private:
    int snake_length = 8;
    int color_segment_length = 2;
    std::vector<cartesianPos_t> allocated_diodes;
    int64_t one_frame_time_us;
    cartesianPos_t start_pos;
    cartesianPos_t last_pos;
    cubeDim_t cube_dimensions = {0};
    Direction dir;
    Direction last_dir;
private:
    bool isHitTheWall();
    bool isChangeDirection();
    void drawStartingPoint();
    Direction drawFirstDirection();
    Direction drawDirection(std::vector<Direction> dirs);
    std::vector<Direction> getAllCurrentPossibleDirs();
    void moveSnake(Direction dir);
    void render(LedCube* cube);
};
