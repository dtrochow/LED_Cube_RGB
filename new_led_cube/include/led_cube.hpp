#include "led_matrix.hpp"

/**
 * Translates the LedMatrix memory,
 * to the memory of LedCube
 */
class LedMatrixToCubeMemoryHub {
public:
    LedMatrixToCubeMemoryHub();
    ~LedMatrixToCubeMemoryHub() {};
public:
    virtual void writeMatrixToMemory(LedMatrix* led_matrix);
};

class AnalogLedCubeRGB4x4x4MCP23017 : LedMatrixToCubeMemoryHub {
public:
    AnalogLedCubeRGB4x4x4MCP23017();
    ~AnalogLedCubeRGB4x4x4MCP23017() {};
public:
    void writeMatrixToMemory(LedMatrix* led_matrix) override;
};

/**
 * LedCube
 * Requires the previously created two objects:
 * 1. LedMatrix - which is related to physical leds layout on real LedCube
 * 2. LedMatrixToCubeMemoryHub - which is responsible for translating LedMatrix memory to 
 *                               hardware specific LedCube memory
 */
class LedCube {
public:
    LedCube(LedMatrixToCubeMemoryHub* matrix_to_memeory_hub, LedMatrix* led_matrix);
    ~LedCube() {};
public:
    /**
     * Updates all led states from LedMatrix to physical LedCube.
     * Writes all memory from LedMatrix to LedCube hardware.
     */
    virtual void render();
    /**
     * Operations on matrix can be performed directly through LedMatrix APIs.
     */
    LedMatrix* matrix;
private:
    LedMatrixToCubeMemoryHub* led_cube_memeory_hub;
};

class LedCubeAnalog4x4x4 : LedCube {
public:
    LedCubeAnalog4x4x4();
    ~LedCubeAnalog4x4x4();
public:
    void render() override;
};
