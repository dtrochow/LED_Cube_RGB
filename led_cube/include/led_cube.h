#include "mcp23017.h"
#include "hardware/i2c.h"

#define LED_CUBE_RGB_X      4
#define LED_CUBE_RGB_Y      4
#define LED_CUBE_RGB_Z      4

#define MCP23017_COLUMNS_ADDR   ADDRESS2
#define MCP23017_ROWS_ADDR      ADDRESS1
#define MCP23017_I2C_INST       i2c1

typedef enum
{
    NONE = 0,
    RED,
    GREEN,
    BLUE,
    MAGENTA,
    CYAN,
    YELLOW,
    WHITE
} diodeColor_e;

typedef enum
{
    X_PLANE = 0,
    Y_PLANE,
    Z_PLANE
} planeAxis_e;

typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t z;
} ledPos_t;

typedef struct
{
    uint8_t red_output;
    uint8_t green_output;
    uint8_t blue_output;
    uint8_t column;
} rgbLedOutputs_t;


typedef struct
{
    bool red;
    bool green;
    bool blue;
} rgbValues_t;


typedef struct
{
    bool red;
    bool green;
    bool blue;
    diodeColor_e color;
    bool enabled;
    uint8_t red_pin;
    uint8_t green_pin;
    uint8_t blue_pin;
    uint8_t colunm_pin;
} ledRGB_t;

/**
 * @brief 
 * 
 */
typedef enum
{
    C_00 = 0, // mcp23017 output number in range 0-15
    C_01 = 1,
    C_02 = 2,
    C_03 = 3,
    C_10 = 4,
    C_11 = 5,
    C_12 = 6,
    C_13 = 7,
    C_20 = 8,
    C_21 = 9,
    C_22 = 10,
    C_23 = 11,
    C_30 = 12,
    C_31 = 13,
    C_32 = 14,
    C_33 = 15
} cubeColumns_e;

/**
 * @brief 
 * 
 */
typedef enum
{
    RED_0   = 0,
    GREEN_0 = 1,
    BLUE_0  = 2,
    RED_1   = 3,
    GREEN_1 = 4,
    BLUE_1  = 5,
    RED_2   = 6,
    GREEN_2 = 7,
    BLUE_2  = 8,
    RED_3   = 9,
    GREEN_3 = 10,
    BLUE_3  = 11
} cubeRows_e;

void lc_init(i2c_inst_t *i2c);

void lc_enable_diode(uint8_t x_pos, uint8_t y_pos, uint8_t z_pos, diodeColor_e color, bool update);
void lc_disable_layer(uint8_t z_pos, bool update);
void lc_disable_all_layers(bool update);
void lc_disable_one_diode(uint8_t x_pos, uint8_t y_pos, uint8_t z_pos,  bool update);
void lc_enable_plane(planeAxis_e orient, uint8_t pos, diodeColor_e color, bool update);
void lc_disable_plane(planeAxis_e orient, uint8_t pos, bool update);
void lc_enable_column(uint8_t x, uint8_t y, diodeColor_e color, bool update);
void lc_disable_column(uint8_t x, uint8_t y, bool update);