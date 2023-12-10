#include <array>

#include "fan.hpp"

void Fan::run(LedCube& cube, const AnimationSpeed speed, const int iterations)
{
    one_frame_time_us = 300000 / utils::to_underlying(speed);

    for (int i = 0; i < iterations; i++)
    {
        render(cube);
    }
}

void Fan::render(LedCube& cube) const
{
    const int cube_height = cube.getDimension(Dimension::Z);

    constexpr int number_of_possible_positions = 6;
    constexpr int diodes_enabled_per_frame = 4;

    // @TODO: Make calculation of blade positions dependent on cube XY dimensions
    const std::vector<std::vector<XYCoordinates>> blade_positions {{
        { XYCoordinates(0,0), XYCoordinates(1,1), XYCoordinates(2,2), XYCoordinates(3,3) },
        { XYCoordinates(0,1), XYCoordinates(1,1), XYCoordinates(2,2), XYCoordinates(3,2) },
        { XYCoordinates(0,2), XYCoordinates(1,2), XYCoordinates(2,1), XYCoordinates(3,1) },
        { XYCoordinates(0,3), XYCoordinates(1,2), XYCoordinates(2,1), XYCoordinates(3,0) },
        { XYCoordinates(1,3), XYCoordinates(1,2), XYCoordinates(2,1), XYCoordinates(2,0) },
        { XYCoordinates(2,3), XYCoordinates(2,2), XYCoordinates(1,1), XYCoordinates(1,0) },
    }};

    for (int i = 0; i < blade_positions.size(); i++)
    {
        const absolute_time_t start = get_absolute_time();
        while (absolute_time_diff_us(start, get_absolute_time()) < one_frame_time_us) {
            const int lower_blade_pos_index = i;
            // Spin backwards
            const int upper_blade_pos_index = (blade_positions.size() - 1) - i;
            render_frame(cube, blade_positions[lower_blade_pos_index], 0, (cube_height/2), Color::GREEN);
            render_frame(cube, blade_positions[upper_blade_pos_index], (cube_height/2), (cube_height/2), Color::BLUE);
        }
    }
}

void Fan::render_frame(LedCube& cube, const std::vector<XYCoordinates>& frame, int start, int height, Color color) const
{
    for (const auto& diode_pos: frame)
    {
        MatrixOperation enable_column = EnableColumn(ColumnCoordinates(Plane::Z, diode_pos.x, diode_pos.y, height, start));
        cube.action(enable_column, LedSwitch::ENABLE, color);
    }
    cube.render();

    for (const auto& diode_pos: frame)
    {
        MatrixOperation enable_column = EnableColumn(ColumnCoordinates(Plane::Z, diode_pos.x, diode_pos.y, height, start));
        cube.action(enable_column, LedSwitch::DISABLE);
    }
    cube.render();
}
