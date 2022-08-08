#include "animations.hpp"
#include "led_matrix.hpp"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"

/* -------------------------------------------------------------------------- */
/*                             Static definitions                             */
/* -------------------------------------------------------------------------- */

static void la_seed_random_from_rosc(void);

/* -------------------------------------------------------------------------- */

/**
 * Inserts random seed from HW for random number generation.
 */
static void la_seed_random_from_rosc(void) {
    uint32_t random = 0x811c9dc5;
    uint8_t next_byte = 0;
    volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);

    for (int i = 0; i < 16; i++) {
        for (int k = 0; k < 8; k++) {
            next_byte = (next_byte << 1) | (*rnd_reg & 1);
        }

        random ^= next_byte;
        random *= 0x01000193;
    }

    srand(random);
}

/* -------------------------------------------------------------------------- */
/*                              Animations Runner                             */
/* -------------------------------------------------------------------------- */

AnimationsRunner::AnimationsRunner(LedCube* cube_) {
    cube = cube_;
    animations = {
        { AnimationType::ALL_LEDS_ALL_COLORS, new AllLedsAllColors() },
        { AnimationType::TENNIS_BALL_SEAM,    new TennisBallSeam()   }
    };
}

void AnimationsRunner::run(AnimationType a_type, AnimationSpeed speed) {
    animations[a_type]->run(cube, speed);
}

/* -------------------------------------------------------------------------- */
/*                                 Animations                                 */
/* -------------------------------------------------------------------------- */

/**
 * Enables all leds one by one with all available colors.
 */
void AllLedsAllColors::run(LedCube* cube, AnimationSpeed speed) {
    for (auto color: all_colors) {
        for (int z = 0; z < cube->getDimension(Dimension::Z); z++) {
            for (int y = 0; y < cube->getDimension(Dimension::Y); y++) {
                for (int x = 0; x < cube->getDimension(Dimension::X); x++) {
                    CartesianCoordinates cr(x, y, z);
                    EnableSingle enable_single(&cr);
                    cube->action(&enable_single, LedSwitch::ENABLE, color);
                    cube->render();
                    sleep_ms(100);
                    cube->action(&enable_single, LedSwitch::DISABLE);
                    cube->render();
                }
            }
        }
    }
}

/**
 * Enables consecutive leds in the path of cubic tennis ball seam.
 */
void TennisBallSeam::run(LedCube* cube, AnimationSpeed speed) {
    // TODO: Add function body
}
