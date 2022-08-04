#pragma once

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
