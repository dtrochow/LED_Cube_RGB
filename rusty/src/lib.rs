#![no_std]

use core::panic::PanicInfo;

#[panic_handler]
fn panik(_panic_info: &PanicInfo) -> !{
    loop {
    }
}

extern{
    fn gpio_put_explicit(pin: i32, state: bool) -> i32;
    fn sleep_ms(time: i32) -> i32;
}

#[no_mangle]
pub extern "C" fn blink(pin: i32, time: i32 ) -> i32{
    unsafe{
        gpio_put_explicit(pin, true);
        sleep_ms(time);
        gpio_put_explicit(pin, false);
        sleep_ms(time);
    }
    1
}