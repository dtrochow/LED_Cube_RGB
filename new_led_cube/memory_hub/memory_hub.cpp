#include "memory_hub.hpp"
#include "mcp23017.h"

// /**
//  * @brief
//  *
//  */
// typedef enum {
//     C_00 = 0, // mcp23017 output number in range 0-15
//     C_01 = 1,
//     C_02 = 2,
//     C_03 = 3,
//     C_10 = 4,
//     C_11 = 5,
//     C_12 = 6,
//     C_13 = 7,
//     C_20 = 8,
//     C_21 = 9,
//     C_22 = 10,
//     C_23 = 11,
//     C_30 = 12,
//     C_31 = 13,
//     C_32 = 14,
//     C_33 = 15
// } cubeColumns_e;

// /**
//  * @brief
//  *
//  */
// typedef enum {
//     RED_0   = 0,
//     GREEN_0 = 1,
//     BLUE_0  = 2,
//     RED_1   = 3,
//     GREEN_1 = 4,
//     BLUE_1  = 5,
//     RED_2   = 6,
//     GREEN_2 = 7,
//     BLUE_2  = 8,
//     RED_3   = 9,
//     GREEN_3 = 10,
//     BLUE_3  = 11
// } cubeRows_e;

AnalogLedCubeRGB4x4x4MCP23017::AnalogLedCubeRGB4x4x4MCP23017() {
    memset(ledCube, 0, sizeof(ledRGB_t));
    mcp_enable(ADDRESS1);
    mcp_enable(ADDRESS2);
    mcp_set_mode(i2c1, ADDRESS1, GPIOA, 0x00);
    mcp_set_mode(i2c1, ADDRESS1, GPIOB, 0x00);
    mcp_set_mode(i2c1, ADDRESS2, GPIOA, 0x00);
    mcp_set_mode(i2c1, ADDRESS2, GPIOB, 0x00);

    mcp_write(i2c1, ADDRESS1, GPIOA, 0xFF, true);
    mcp_write(i2c1, ADDRESS1, GPIOB, 0xFF, true);
    mcp_write(i2c1, ADDRESS2, GPIOA, 0x00, true);
    mcp_write(i2c1, ADDRESS2, GPIOB, 0x00, true);
}

void AnalogLedCubeRGB4x4x4MCP23017::writeMatrixToMemory(LedMatrix* led_matrix) {

}
