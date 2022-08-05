#pragma once

#include "led_matrix.hpp"
#include "led_rgb.hpp"
#include "mcp23017.h"
#include <utility>
#include <map>

/**
 * Translates the LedMatrix memory,
 * to the memory of LedCube
 */
class LedMatrixToCubeMemoryHub {
public:
    virtual ~LedMatrixToCubeMemoryHub() {};
public:
    virtual void writeMatrixToMemory(LedMatrix* led_matrix) = 0;
};

class AnalogLedCubeRGB4x4x4MCP23017 : public LedMatrixToCubeMemoryHub {
public:
    AnalogLedCubeRGB4x4x4MCP23017(i2c_inst_t *i2c_);
    ~AnalogLedCubeRGB4x4x4MCP23017() {};
public:
    void writeMatrixToMemory(LedMatrix* led_matrix) override;
private:
    const uint8_t mcp_columns_addr = ADDRESS2;
    const uint8_t mcp_rows_addr    = ADDRESS1;
    const std::size_t columns_count = 16;
    const std::size_t rows_count = 12;
private:
    void hardware_init();
    std::map<std::pair <int, int>, int> columns_mapping;
    i2c_inst_t *i2c;
    std::vector<bool> columns;
    std::vector<bool> rows;
};
