#include "mcp23017.h"

#define LED_CUBE_RGB_X      4
#define LED_CUBE_RGB_Y      4
#define LED_CUBE_RGB_Z      4

typedef struct
{
    bool red;
    bool green;
    bool blue;
} ledRGB_t;
