#include "led_animations_api.h"
#include <stdlib.h>
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"

static void la_get_random_led(ledPos_t *position, int *color, int color_size);
static void la_seed_random_from_rosc(void);

static void la_seed_random_from_rosc(void)
{
    uint32_t random = 0x811c9dc5;
    uint8_t next_byte = 0;
    volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);

    for (int i = 0; i < 16; i++)
    {
        for (int k = 0; k < 8; k++) 
        {
            next_byte = (next_byte << 1) | (*rnd_reg & 1);
        }
        random ^= next_byte;
        random *= 0x01000193;
    }

  srand(random);
} 

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

void la_spinning_columns(int delay_ms)
{
    const static uint8_t color[7] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};

    lc_disable_all_layers(true);

    for(int c = 0; c < 7; c++)
    {
        int start = 0;
        int end = LED_CUBE_RGB_X-1;
        
        do
        {
            for(int x = start; x <= end; x ++)
            {
                lc_enable_column(x, start, color[c], true);
                sleep_ms(delay_ms);
                lc_disable_column(x, start, true);
            }
            for(int y = start+1; y <= end; y++)
            {
                lc_enable_column(end, y, color[c], true);
                sleep_ms(delay_ms);
                lc_disable_column(end, y, true);
            }
            for(int x = end-1; x >= start; x--)
            {
                lc_enable_column(x, end, color[c], true);
                sleep_ms(delay_ms);
                lc_disable_column(x, end, true);
            }
            for(int y = end-1; y >= start+1; y--)
            {
                lc_enable_column(0, y, color[c], true);
                sleep_ms(delay_ms);
                lc_disable_column(0, y, true);
            }
            start += 1;
            end -= 1;
        } while(start < end);
    }
}

// 4x4x4
void la_horizontal_spinning_line(int delay_ms, uint8_t layer)
{
    const static uint8_t color[7] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};

    lc_disable_all_layers(true);

    uint8_t led_to_enable[6][4][2] = {
        {{0, 0}, {1, 1}, {2, 2}, {3, 3}},
        {{0, 1}, {1, 1}, {2, 2}, {3, 2}},
        {{0, 2}, {1, 2}, {2, 1}, {3, 1}},
        {{3, 0}, {2, 1}, {1, 2}, {0, 3}},
        {{1, 3}, {1, 2}, {2, 1}, {2, 0}},
        {{2, 3}, {2, 2}, {1, 0}, {1, 1}}
    };

    for(int c = 0; c < 7; c++)
    {
        for(int i = 0; i < 10; i ++)
        {
            for(int j = 0; j < 6; j ++)
            {
                for(int d = 0; d < 4; d ++)
                {
                    if(layer == 255)
                    {
                        lc_enable_column(led_to_enable[j][d][0], led_to_enable[j][d][1], color[c], true);
                    }
                    else if(layer < LED_CUBE_RGB_Z)
                    {
                        lc_enable_diode(led_to_enable[j][d][0], led_to_enable[j][d][1], layer, color[c], true);
                    }
                }
                sleep_ms(delay_ms);
                for(int d = 0; d < 4; d ++)
                {
                    if(layer == LED_CUBE_RGB_Z)
                    {
                        lc_disable_column(led_to_enable[j][d][0], led_to_enable[j][d][1], true);
                    }
                    else if(layer < LED_CUBE_RGB_Z)
                    {
                        lc_disable_one_diode(led_to_enable[j][d][0], led_to_enable[j][d][1], layer, true);
                    }
                }
            }
        }
    }
}

void la_rnd_led_rnd_color(int delay_ms, int repetitions, uint8_t *colors)
{
    lc_disable_all_layers(true);

    ledPos_t led_pos;
    int color;

    for(int i = 0; i < repetitions; i++)
    {
        la_get_random_led(&led_pos, &color, sizeof(colors));
        lc_enable_diode(led_pos.x, led_pos.y, led_pos.z, colors[color], true);
        sleep_ms(delay_ms);
        lc_disable_one_diode(led_pos.x, led_pos.y, led_pos.z, true);
    }
}

static void la_get_random_led(ledPos_t *position, int *color, int color_size)
{
    la_seed_random_from_rosc();
    position->x = rand()%LED_CUBE_RGB_X;
    la_seed_random_from_rosc();
    position->y = rand()%LED_CUBE_RGB_Y;
    la_seed_random_from_rosc();
    position->z = rand()%LED_CUBE_RGB_Z;
    la_seed_random_from_rosc();
    *color = rand()%color_size;
}

static void la_get_random_layer_pos(ledPos_t *position)
{
    la_seed_random_from_rosc();
    position->x = rand()%LED_CUBE_RGB_X;
    la_seed_random_from_rosc();
    position->y = rand()%LED_CUBE_RGB_Y;
}

void la_led_bomb(int delay_ms)
{
    static uint8_t colors[7] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};

    lc_disable_all_layers(true);

    ledPos_t led_pos;
    int color;

    la_get_random_led(&led_pos, &color, sizeof(colors));
    lc_enable_diode(led_pos.x, led_pos.y, led_pos.z, colors[color], true);

    int border = 0;
    while(true)
    {
        border ++;
        if((led_pos.y - border <= 0)                &&
           (led_pos.y + border > LED_CUBE_RGB_Y)    &&
           (led_pos.x - border <= 0)                &&
           (led_pos.x + border > LED_CUBE_RGB_X)
        )
        {
            border = 0;
            break;
        }
        sleep_ms(delay_ms);
        if(led_pos.x + border < LED_CUBE_RGB_X)
        {
            lc_enable_diode(led_pos.x + border, led_pos.y, led_pos.z, colors[color], true);
        }
        if(led_pos.y + border < LED_CUBE_RGB_Y)
        {
            lc_enable_diode(led_pos.x, led_pos.y + border, led_pos.z, colors[color], true);
        }
        if(led_pos.x - border >= 0)
        {
            lc_enable_diode(led_pos.x - border, led_pos.y, led_pos.z, colors[color], true);
        }
        if(led_pos.y - border >= 0)
        {
            lc_enable_diode(led_pos.x, led_pos.y - border, led_pos.z, colors[color], true);
        }
    }
}

void la_rain(int delay_ms)
{
    // static uint8_t colors[7] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};
    static uint8_t colors[1] = {CYAN};

    while(true)
    {
        lc_disable_all_layers(true);

        ledPos_t led_pos;
        int color;
        verticalDirection_e direction = DOWN;

        la_get_random_led(&led_pos, &color, sizeof(colors));

        for(int i = LED_CUBE_RGB_Z - 1; i >= 0; i --)
        {
            lc_enable_diode(led_pos.x, led_pos.y, i, colors[color], true);
            sleep_ms(delay_ms);
            lc_disable_one_diode(led_pos.x, led_pos.y, i, true);
        }        
    }
}

void la_spinning_plane(int delay_ms)
{
    static int color = BLUE;

    lc_disable_all_layers(true);

    lc_enable_plane(X_PLANE, 0, color, true);
    sleep_ms(delay_ms);
    lc_disable_plane(X_PLANE, 0, true);

    lc_enable_plane(Y_PLANE, 0, color, true);
    sleep_ms(delay_ms);
    lc_disable_plane(Y_PLANE, 0, true);

    lc_enable_plane(X_PLANE, LED_CUBE_RGB_X-1, color, true);
    sleep_ms(delay_ms);
    lc_disable_plane(X_PLANE, LED_CUBE_RGB_X-1, true);

    lc_enable_plane(Y_PLANE, LED_CUBE_RGB_Y-1, color, true);
    sleep_ms(delay_ms);
    lc_disable_plane(Y_PLANE, LED_CUBE_RGB_Y-1, true);

}

void la_spinning_four_columns(int delay_ms)
{
    static int color = WHITE;

    for (int i = 0; i < 3; i ++)
    {
        lc_enable_column(0, i, color, true);
        lc_enable_column(i, LED_CUBE_RGB_Y-1, color, true);
        lc_enable_column(LED_CUBE_RGB_X-1, LED_CUBE_RGB_Y-1-i, color, true);
        lc_enable_column(LED_CUBE_RGB_X-1-i, 0, color, true);
        sleep_ms(delay_ms);
        lc_disable_column(0, i, true);
        lc_disable_column(i, LED_CUBE_RGB_Y-1, true);
        lc_disable_column(LED_CUBE_RGB_X-1, LED_CUBE_RGB_Y-1-i, true);
        lc_disable_column(LED_CUBE_RGB_X-1-i, 0, true);
    }
}

void la_test_all_diodes(int delay_ms)
{
    static uint8_t colors[7] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};

    for(int i = 0; i < sizeof(colors); i++)
    {
        lc_enable_whole_cube(colors[i]);
        sleep_ms(delay_ms);        
    }

}