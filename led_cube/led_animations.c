#include "led_animations_api.h"

void la_test_all_leds(int delay_ms)
{
    const static uint8_t color[4] = {RED, GREEN, BLUE, WHITE};

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

void la_planes_animation(int delay_ms)
{
    const static uint8_t color[7] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};
    const static uint8_t sizes[3] = {LED_CUBE_RGB_X, LED_CUBE_RGB_Y, LED_CUBE_RGB_Z};
    const static uint8_t planes[3] = {X_PLANE, Y_PLANE, Z_PLANE};

    lc_disable_all_layers(true);

    for(int c = 0; c < 7; c++)
    {
        for(int i = 0; i < 3; i++)
        {
            for(int s = 0; s < sizes[i]; s++)
            {
                lc_enable_plane(planes[i], s, color[c], true);
                sleep_ms(delay_ms);
                lc_disable_plane(planes[i], s, true);
            }
            for(int s = sizes[i]-1; s >= 0; s--)
            {
                lc_enable_plane(planes[i], s, color[c], true);
                sleep_ms(delay_ms);
                lc_disable_plane(planes[i], s, true);
            }
        }
    }
}

// void la_rnd_led_rnd_color(int delay_ms)
// {
//     lc_disable_all_layers(true);

//     const static uint8_t color[7] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};


// }
