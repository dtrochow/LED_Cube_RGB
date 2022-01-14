#include "led_cube.h"

static ledRGB_t ledCube[LED_CUBE_RGB_X][LED_CUBE_RGB_Y][LED_CUBE_RGB_Z];

static rgbValues_t lc_get_led_states_from_color(diodeColor_e color);
static void lc_update_one_rgbled_state(ledPos_t pos);
static void lc_update_layer_state(uint8_t z_pos);
static void lc_write_one_rgbled_state(rgbValues_t rgbValues, ledRGB_t *ledRGB);
static rgbLedOutputs_t lc_get_led_outputs(ledPos_t pos);

void lc_init(i2c_inst_t *i2c)
{
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

void lc_enable_diode(uint8_t x_pos, uint8_t y_pos, uint8_t z_pos, diodeColor_e color, bool update)
{
    lc_write_one_rgbled_state(lc_get_led_states_from_color(color), &ledCube[x_pos][y_pos][z_pos]);
    ledCube[x_pos][y_pos][z_pos].color = color;
    ledCube[x_pos][y_pos][z_pos].enabled = true;

    if(true == update)
    {
        ledPos_t pos = {x_pos, y_pos, z_pos};
        // Update IO state usin mcp23017
        lc_update_one_rgbled_state(pos);        
    }
}

void lc_disable_one_diode(uint8_t x_pos, uint8_t y_pos, uint8_t z_pos,  bool update)
{
    rgbValues_t values;
    values.red = false;
    values.green = false;
    values.blue = false;
    lc_write_one_rgbled_state(values , &ledCube[x_pos][y_pos][z_pos]);
    ledCube[x_pos][y_pos][z_pos].color = NONE;
    ledCube[x_pos][y_pos][z_pos].enabled = false;

    if(true == update)
    {
        ledPos_t pos = {x_pos, y_pos, z_pos};
        // Update IO state usin mcp23017
        lc_update_one_rgbled_state(pos);        
    }
}

void lc_disable_layer(uint8_t z_pos, bool update)
{
    ledRGB_t led;
    led.red = false;
    led.green = false;
    led.blue = false;
    led.color = NONE;
    led.enabled = false;

    for(int x = 0; x < LED_CUBE_RGB_X; x++)
    {
        for(int y = 0; y < LED_CUBE_RGB_Y; y ++)
        {
            ledCube[x][y][z_pos]= led;
        }
    }

    if(true == update)
    {
        // Update IO state usin mcp23017
        lc_update_layer_state(z_pos);    
    }
}

void lc_disable_all_layers(bool update)
{
    for(int i = 0; i < LED_CUBE_RGB_Z; i++)
    {
        lc_disable_layer(i, update);
    }
}

static void lc_update_one_rgbled_state(ledPos_t pos)
{
    rgbLedOutputs_t rgbLedOutputs = lc_get_led_outputs(pos);
    ledRGB_t led = ledCube[pos.x][pos.y][pos.z];

    if(true == led.enabled)
    {
        mcp_write_single(MCP23017_I2C_INST, MCP23017_COLUMNS_ADDR, rgbLedOutputs.column, HIGH, true);
    }
    else if(false == led.enabled)
    {
        mcp_write_single(MCP23017_I2C_INST, MCP23017_COLUMNS_ADDR, rgbLedOutputs.column, LOW, true);
    }
    mcp_write_single(MCP23017_I2C_INST, MCP23017_ROWS_ADDR, rgbLedOutputs.green_output, !led.green, true);
    mcp_write_single(MCP23017_I2C_INST, MCP23017_ROWS_ADDR, rgbLedOutputs.red_output, !led.red, true);
    mcp_write_single(MCP23017_I2C_INST, MCP23017_ROWS_ADDR, rgbLedOutputs.blue_output, !led.blue, true);
}

static void lc_update_layer_state(uint8_t z_pos)
{
    for(int x = 0; x < LED_CUBE_RGB_X; x++)
    {
        for(int y = 0; y < LED_CUBE_RGB_Y; y ++)
        {
            ledPos_t pos = {x, y, z_pos};
            lc_update_one_rgbled_state(pos);
        }
    }
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

void lc_enable_plane(planeAxis_e orient, uint8_t pos, diodeColor_e color, bool update)
{
    switch(orient)
    {
        case X_PLANE:
            for(int z = 0; z < LED_CUBE_RGB_Z; z ++)
            {
                for(int y = 0; y < LED_CUBE_RGB_Y; y ++)
                {
                    lc_enable_diode(pos, y, z, color, update);
                }
            }
            break;  
        case Y_PLANE:
            for(int z = 0; z < LED_CUBE_RGB_Z; z ++)
            {
                for(int x = 0; x < LED_CUBE_RGB_X; x ++)
                {
                    lc_enable_diode(x, pos, z, color, update);
                }
            }           
            break;
        case Z_PLANE:
            for(int x = 0; x < LED_CUBE_RGB_X; x ++)
            {
                for(int y = 0; y < LED_CUBE_RGB_Y; y ++)
                {
                    lc_enable_diode(x, y, pos, color, update);
                }
            }
            break;
    }
}

void lc_disable_plane(planeAxis_e orient, uint8_t pos, bool update)
{
    switch(orient)
    {
        case X_PLANE:
            for(int z = 0; z < LED_CUBE_RGB_Z; z ++)
            {
                for(int y = 0; y < LED_CUBE_RGB_Y; y ++)
                {
                    lc_disable_one_diode(pos, y, z, update);
                }
            }
            break;  
        case Y_PLANE:
            for(int z = 0; z < LED_CUBE_RGB_Z; z ++)
            {
                for(int x = 0; x < LED_CUBE_RGB_X; x ++)
                {
                    lc_disable_one_diode(x, pos, z, update);
                }
            }           
            break;
        case Z_PLANE:
            for(int x = 0; x < LED_CUBE_RGB_X; x ++)
            {
                for(int y = 0; y < LED_CUBE_RGB_Y; y ++)
                {
                    lc_disable_one_diode(x, y, pos, update);
                }
            }
            break;
    }
}

void lc_update_all_diodes(void)
{

}
