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
    virtual ~LedCube() {};
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
protected:
    LedMatrixToCubeMemoryHub* led_cube_memory_hub;
};

class LedCubeAnalog4x4x4 : LedCube {
public:
    LedCubeAnalog4x4x4(LedMatrixToCubeMemoryHub* matrix_to_mmeory_hub, LedMatrix* led_matrix);
    ~LedCubeAnalog4x4x4() {};
public:
    void render() override;
};
