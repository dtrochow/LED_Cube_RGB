#include <memory>
#include "animation_utils.hpp"

class Snake : public Animation {
public:
    Snake();
    ~Snake() override {};
public:
    void run(LedCube& cube, const AnimationSpeed speed, const int iterations) override;
private:
    int snake_length = 13;
    int color_segment_length = 7;
    uint snake_move_counter = 0;
    Color next_color;
    std::vector<std::pair<cartesianPos_t, Color> > allocated_diodes;
    int64_t one_frame_time_us;
    cartesianPos_t start_pos;
    cartesianPos_t last_pos;
    cubeDim_t cube_dimensions;
    Direction dir;
    Direction last_dir;
private:
    bool isHitTheWall() const;
    bool isChangeDirection() const;
    void drawStartingPoint();
    Direction drawFirstDirection() const;
    Direction drawDirection(std::vector<Direction> const& dirs) const;
    std::vector<Direction> getAllCurrentPossibleDirs() const;
    void moveSnake(Direction dir);
    void render(LedCube& cube) const;
    void init_action_matrix();
};
