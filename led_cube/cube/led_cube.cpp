#include "led_cube.hpp"
#include "memory_hub.hpp"
#include "led_matrix.hpp"

LedCubeAnalog4x4x4::LedCubeAnalog4x4x4(AnalogLedCubeRGB4x4x4MCP23017* matrix_to_memory_hub, LedCreator *ledFactory) :
    matrix(4, 4, 4, ledFactory)
{
    led_cube_memory_hub = matrix_to_memory_hub;
}

void LedCubeAnalog4x4x4::render() {
    led_cube_memory_hub->writeMatrixToMemory(&matrix);
}

void LedCubeAnalog4x4x4::action(MatrixOperation* operation, LedSwitch switch_state, Color color) {
    matrix.action(operation, switch_state, color);
}

void LedCubeAnalog4x4x4::reset() {
    matrix.reset();
}
