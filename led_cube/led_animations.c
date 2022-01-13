#include "led_animations_api.h"

void la_test_all_leds(int delay_ms)
{
    uint8_t color[4] = {RED, GREEN, BLUE, WHITE};

    lc_disable_all_layers(true);
    
    for(int c = 0; c < 4; c++)
    {
        for(int z = 0; z < LED_CUBE_RGB_Z; z++)
        {
            for(int x = 0; x < LED_CUBE_RGB_X; x++)
            {
                for(int y = 0; y < LED_CUBE_RGB_Y; y++)
                {
                    lc_enable_diode(x, y, z, color[c], true);
                    sleep_ms(delay_ms);
                    lc_disable_one_diode(x, y, z, true);
                }
            }
        }
    }
}
