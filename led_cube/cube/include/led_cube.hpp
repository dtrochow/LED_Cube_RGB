#pragma once

#include "led_matrix.hpp"
#include "memory_hub.hpp"

/**
 * LedCube
 * Requires the previously created two objects:
 * 1. LedMatrix - which is related to physical leds layout on real LedCube
 * 2. LedMatrixToCubeMemoryHub - which is responsible for translating LedMatrix memory to 
 *                               hardware specific LedCube memory
 */
class LedCube {
public:
    LedCube(int x_size, int y_size, int z_size, LedCreator* factory) :
        matrix(x_size, y_size, z_size, factory) {}
    virtual ~LedCube() {};
public:
    /**
     * Updates all led states from LedMatrix to physical LedCube.
     * Writes all memory from LedMatrix to LedCube hardware.
     */
    virtual void render() = 0;
    virtual void action(MatrixOperation* operation, LedSwitch switch_state, Color color = Color::NONE) = 0;
    virtual void reset() = 0;
    int getDimension(Dimension dim);
protected:
    LedMatrixToCubeMemoryHub* led_cube_memory_hub;
    LedMatrix matrix;
};

class LedCubeAnalog4x4x4 : public LedCube {
public:
    LedCubeAnalog4x4x4(AnalogLedCubeRGB4x4x4MCP23017* matrix_to_mmeory_hub, LedCreator *ledFactory);
    ~LedCubeAnalog4x4x4() {};
public:
    void render() override;
    void action(MatrixOperation* operation, LedSwitch switch_state, Color color = Color::NONE) override;
    void reset() override;
};
