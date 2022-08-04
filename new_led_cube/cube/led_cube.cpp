#include "led_cube.hpp"
#include "memory_hub.hpp"

LedCubeAnalog4x4x4::LedCubeAnalog4x4x4(LedMatrixToCubeMemoryHub* matrix_to_memory_hub, LedMatrix* led_matrix) {
    matrix = led_matrix;
    led_cube_memory_hub = matrix_to_memory_hub;
}

void LedCubeAnalog4x4x4::render() {
    led_cube_memory_hub->writeMatrixToMemory(matrix);
}
