#include "memory_hub.hpp"


AnalogLedCubeRGB4x4x4MCP23017::AnalogLedCubeRGB4x4x4MCP23017(i2c_inst_t *i2c_) :
    i2c(i2c_)
{
    // 1. Initialize hardware
    hardware_init();
    // Cube columns to HW outputs mapping
    columns_mapping = {
        { { 0, 0 }, 0  },
        { { 0, 1 }, 1  },
        { { 0, 2 }, 2  },
        { { 0, 3 }, 3  },
        { { 1, 0 }, 4  },
        { { 1, 1 }, 5  },
        { { 1, 2 }, 6  },
        { { 1, 3 }, 7  },
        { { 2, 0 }, 8  },
        { { 2, 1 }, 9  },
        { { 2, 2 }, 10 },
        { { 2, 3 }, 11 },
        { { 3, 0 }, 12 },
        { { 3, 1 }, 13 },
        { { 3, 2 }, 14 },
        { { 3, 3 }, 15 }
    };
    columns.resize(columns_count, false);
    rows.resize(rows_count, false);
}

void AnalogLedCubeRGB4x4x4MCP23017::hardware_init()
{
    mcp_enable(ADDRESS1);
    mcp_enable(ADDRESS2);

    mcp_set_mode(i2c, ADDRESS1, GPIOA, 0x00);
    mcp_set_mode(i2c, ADDRESS1, GPIOB, 0x00);
    mcp_set_mode(i2c, ADDRESS2, GPIOA, 0x00);
    mcp_set_mode(i2c, ADDRESS2, GPIOB, 0x00);

    mcp_write(i2c, ADDRESS1, GPIOA, 0xFF, true);
    mcp_write(i2c, ADDRESS1, GPIOB, 0xFF, true);
    mcp_write(i2c, ADDRESS2, GPIOA, 0x00, true);
    mcp_write(i2c, ADDRESS2, GPIOB, 0x00, true);
}

void AnalogLedCubeRGB4x4x4MCP23017::writeMatrixToMemory(LedMatrix& led_matrix)
{
    // Clear buffers
    std::fill(columns.begin(), columns.end(), false);
    std::fill(rows.begin(), rows.end(), false);

    /* Rows                             Columns [XY]
       o . o . o . o  <- BLUE              00 - 01 - 02 - 03
       |   |   |   |                       |    |     |    |
       o . o . o . o  <- GREEN             10 - 11 - 12 - 13
       |   |   |   |                       |    |     |    |
       o . o . o . o  <- RED               20 - 21 - 22 - 23
                                           |    |     |    |
                                           30 - 31 - 32 - 33
     */

    // 3. Update memory buffer basing on led_matrix
    for (int z = 0; z < 4; z++) {
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                std::pair<int, int> pos(x, y);

                if (LedState::ENABLED == led_matrix.leds[x][y][z]->getLedDiodeState(Led::RED)) {
                    rows[(z * 3) + (int)Led::RED] = true;
                    columns[columns_mapping[pos]] = true;
                }

                if (LedState::ENABLED == led_matrix.leds[x][y][z]->getLedDiodeState(Led::GREEN)) {
                    rows[(z * 3) + (int)Led::GREEN] = true;
                    columns[columns_mapping[pos]] = true;
                }

                if (LedState::ENABLED == led_matrix.leds[x][y][z]->getLedDiodeState(Led::BLUE)) {
                    rows[(z * 3) + (int)Led::BLUE] = true;
                    columns[columns_mapping[pos]] = true;
                }
            }
        }
    }

    // 4. Write data to hardware from prepared buffer
    rows.flip(); // Rows outputs have inverted logic

    for (int r = 0; r < rows_count; r++) {
        mcp_write_single(i2c, mcp_rows_addr, r, rows[r], false);
    }

    for (int c = 0; c < columns_count; c++) {
        mcp_write_single(i2c, mcp_columns_addr, c, columns[c], false);
    }

    mcp_update_out_state_all(i2c);
}
