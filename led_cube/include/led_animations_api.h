#include "led_cube.h"

typedef enum {
    UP   = 1,
    DOWN = -1
} verticalDirection_e;

void la_test_all_leds(int delay_ms);
void la_rnd_led_rnd_color(int delay_ms, int repetitions, uint8_t *colors);
void la_planes_animation(int delay_ms);
void la_spinning_columns(int delay_ms);
void la_horizontal_spinning_line(int delay_ms, uint8_t layer);
void la_led_bomb(int delay_ms);
void la_rain(int delay_ms);
void la_spinning_plane(int delay_ms);
void la_spinning_four_columns(int delay_ms);
void la_test_all_diodes(int delay_ms);