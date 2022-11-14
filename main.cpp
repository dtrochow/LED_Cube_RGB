/***************************************
* main.cpp
* rev 1.0 Dominik 2022
*  ************************************/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "led_cube.hpp"
#include "led_matrix.hpp"
#include "memory_hub.hpp"
#include "led_rgb.hpp"
#include "colors_config.hpp"
#include "animation_runner.hpp"

const int I2C_SPEED = 1000000;

#define UART_ID uart1
// #define BAUD_RATE 115200
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 4
#define UART_RX_PIN 5

void core1_interrupt_handler() {
    // 1. Create module, which will handle notifications
    //      - notification can be static - only turn on a specific led
    //      - or dynamic - run an animation
    // 2. There should be separate command to clear all notifications
    // 3. There could be also command to clear only one notification
    //      - e.g. data:
    //            [notificationID, ]
    // notificationID - will identify specific notification, all notifications must have:
    //                * reserved colors
    //                * reserved position of leds, which will indicate the notification
    // Think through if there will be possibiliti of showing more than one notification at the time
    //   - maybe there shoulb be some implemented some FIFO? after dismissing one notification, other will occur?
    //   - maybe there should be implemented some priorities of notifications? only the one with the highest priority will be shown?
    
    // First goal:
    //      1) Send notification to the cube using some command
    //      2) Notification will be shown as RED Led turned on
    //      3) Notification should be able to be dismissed
    // Proposed data frame:
    // data = [notificationId, color, action]
    // notificationId - the id will bi tighten up to the specific position and animation on cube
    // color - color of the notification
    // action - turning on or dissmissing

    // Module functions:
    // - module initialization
    // - data parser, which will fill the structure
    // - running notification action (for now without animations, only enabling/disablin diodes once)

    // Future:
    // - adding possibility of running animations as notification
    while(multicore_fifo_rvalid())
    {
        uint32_t data = multicore_fifo_pop_blocking();
    }
    AnalogLedCreator ledFactory(ledConfig_getColors());
    AnalogLedCubeRGB4x4x4MCP23017 memory_hub(i2c1);
    LedCubeAnalog4x4x4 cube(&memory_hub, &ledFactory);

    CartesianCoordinates cc(1,1,1);
    EnableSingle es(&cc);
    cube.action(&es, LedSwitch::ENABLE, Color::RED);
    cube.render();

    multicore_fifo_clear_irq();
}

void core1_entry() {
    multicore_fifo_clear_irq();
    irq_set_exclusive_handler(SIO_IRQ_PROC1, core1_interrupt_handler);
    irq_set_enabled(SIO_IRQ_PROC1, true);

    while (1) {
        tight_loop_contents();
    }
}

// RX interrupt handler
// Receiving data and sending it to core1 through FIFO, which will trigger the core1 irq
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        multicore_fifo_push_blocking(ch);
    }
}

void initialize_uart() {
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART1_IRQ, on_uart_rx);
    irq_set_enabled(UART1_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);
}

void initialize_i2c() {
    i2c_init(i2c1, I2C_SPEED);
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_set_function(19, GPIO_FUNC_I2C);
}

int main(void) {
    stdio_init_all();
    initialize_uart();
    initialize_i2c();

    // Running second core
    multicore_launch_core1(core1_entry);

    while (1) {
        tight_loop_contents();
    }
}
