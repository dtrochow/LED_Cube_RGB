#include "led_cube.h"

static ledRGB_t ledCube[LED_CUBE_RGB_X][LED_CUBE_RGB_Y][LED_CUBE_RGB_Z];

static rgbValues_t lc_get_led_states_from_color(diodeColor_e color);
static void lc_update_one_rgbled_state(uint8_t columns_address, uint8_t rows_address, ledPos_t pos);
static void lc_write_one_rgbled_state(rgbValues_t rgbValues, ledRGB_t *ledRGB);
static rgbLedOutputs_t lc_get_led_outputs(ledPos_t pos);

void lc_enable_diode(uint8_t x_pos, uint8_t y_pos, uint8_t z_pos, diodeColor_e color, bool update)
{
    ledPos_t pos = {x_pos, y_pos, z_pos};
    ledCube[x_pos][y_pos][z_pos].color = color;
    lc_write_one_rgbled_state(lc_get_led_states_from_color(color), &ledCube[x_pos][y_pos][z_pos]);

    if(true == update)
    {
        // Update IO state usin mcp23017
        lc_update_one_rgbled_state(MCP23017_COLUMNS_ADDR, MCP23017_ROWS_ADDR, pos);        
    }
}

static void lc_update_one_rgbled_state(uint8_t columns_address, uint8_t rows_address, ledPos_t pos)
{
    rgbLedOutputs_t rgbLedOutputs = lc_get_led_outputs(pos);
    ledRGB_t led = ledCube[pos.x][pos.y][pos.z];

    // 1. Get the position
    // 2. Get the led value (each led color for rgb led)
    // 3. Map the rgb led value to output value
    // 4. AND it with current output columns/rows
    // 5. Write it to mcp static structure
    // 6. Update all output values
    mcp_write_single(MCP23017_I2C_INST, MCP23017_COLUMNS_ADDR, rgbLedOutputs.column, HIGH, false);
    mcp_write_single(MCP23017_I2C_INST, MCP23017_ROWS_ADDR, rgbLedOutputs.green_output, led.green, false);
    mcp_write_single(MCP23017_I2C_INST, MCP23017_ROWS_ADDR, rgbLedOutputs.red_output, led.red, false);
    mcp_write_single(MCP23017_I2C_INST, MCP23017_ROWS_ADDR, rgbLedOutputs.blue_output, led.blue, false);
    mcp_update_out_state_all(MCP23017_I2C_INST);
}

static void lc_write_one_rgbled_state(rgbValues_t rgbValues, ledRGB_t *ledRGB)
{
    ledRGB->red = rgbValues.red;
    ledRGB->green = rgbValues.green;
    ledRGB->blue = rgbValues.blue;
}

static rgbValues_t lc_get_led_states_from_color(diodeColor_e color)
{
    rgbValues_t rgbValues;
    switch(color)
    {
        case RED:
            rgbValues.red = true;
            rgbValues.green = false;
            rgbValues.blue = false;
            break;
        case GREEN:
            rgbValues.red = false;
            rgbValues.green = true;
            rgbValues.blue = false;
            break;
        case BLUE:
            rgbValues.red = false;
            rgbValues.green = false;
            rgbValues.blue = true;
            break;
        case CYAN:
            rgbValues.red = false;
            rgbValues.green = true;
            rgbValues.blue = true;
            break;
        case MAGENTA:
            rgbValues.red = true;
            rgbValues.green = false;
            rgbValues.blue = true;
            break;
        case YELLOW:
            rgbValues.red = true;
            rgbValues.green = true;
            rgbValues.blue = false;
            break;
        case WHITE:
            rgbValues.red = true;
            rgbValues.green = true;
            rgbValues.blue = true;
            break;
        default:
            rgbValues.red = false;
            rgbValues.green = false;
            rgbValues.blue = false;
            break;
    }
    return rgbValues;
}

static rgbLedOutputs_t lc_get_led_outputs(ledPos_t pos)
{
    rgbLedOutputs_t rgbLedOutputs;
    uint8_t xy_pos = (10 * pos.x) + pos.y;

    switch(pos.z)
    {
        case 0:
            rgbLedOutputs.red_output = RED_0;
            rgbLedOutputs.green_output = GREEN_0;
            rgbLedOutputs.blue_output = BLUE_0;
            break;
        case 1:
            rgbLedOutputs.red_output = RED_1;
            rgbLedOutputs.green_output = GREEN_1;
            rgbLedOutputs.blue_output = BLUE_1;
            break;
        case 2:
            rgbLedOutputs.red_output = RED_2;
            rgbLedOutputs.green_output = GREEN_2;
            rgbLedOutputs.blue_output = BLUE_2;
            break;
        case 3:
            rgbLedOutputs.red_output = RED_3;
            rgbLedOutputs.green_output = GREEN_3;
            rgbLedOutputs.blue_output = BLUE_3;
            break;
    }
    switch(xy_pos)
    {
        case 0: // 00
            rgbLedOutputs.column = C_00;
            break;
        case 1: // 01
            rgbLedOutputs.column = C_01;
            break;
        case 2: // 02
            rgbLedOutputs.column = C_02;
            break;
        case 3: // 03
            rgbLedOutputs.column = C_03;
            break;
        case 10:
            rgbLedOutputs.column = C_10;
            break;
        case 11:
            rgbLedOutputs.column = C_11;
            break;
        case 12:
            rgbLedOutputs.column = C_12;
            break;
        case 13:
            rgbLedOutputs.column = C_13;
            break;
        case 20:
            rgbLedOutputs.column = C_20;
            break;
        case 21:
            rgbLedOutputs.column = C_21;
            break;
        case 22:
            rgbLedOutputs.column = C_22;
            break;
        case 23:
            rgbLedOutputs.column = C_23;
            break;
        case 30:
            rgbLedOutputs.column = C_30;
            break;
        case 31:
            rgbLedOutputs.column = C_31;
            break;
        case 32:
            rgbLedOutputs.column = C_32;
            break;
        case 33:
            rgbLedOutputs.column = C_33;
            break;
    }
    return rgbLedOutputs;
}
