#include "animation_utils.hpp"

class Snake : public Animation {
public:
    Snake();
    ~Snake() override {};
public:
    void run(LedCube* cube, AnimationSpeed speed, int iterations) override;
private:
    int snake_length = 10;
    int color_segment_length = 5;
    uint snake_move_counter = 0;
    Color next_color;
    std::vector<std::pair<cartesianPos_t, Color>> allocated_diodes;
    int64_t one_frame_time_us;
    cartesianPos_t start_pos;
    cartesianPos_t last_pos;
    cubeDim_t cube_dimensions = {0};
    Direction dir;
    Direction last_dir;
    std::vector<std::vector<std::vector<MatrixOperation*>>> action_matrix;
private:
    bool isHitTheWall();
    bool isChangeDirection();
    void drawStartingPoint();
    Direction drawFirstDirection();
    Direction drawDirection(std::vector<Direction> dirs);
    std::vector<Direction> getAllCurrentPossibleDirs();
    void moveSnake(Direction dir);
    void render(LedCube* cube);
    void init_action_matrix();
};
