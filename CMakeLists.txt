cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

set(projname "led_cube_rgb")

project(${projname} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

include_directories(lib/led_cube)

#Add stepper_motor library
add_library(ledcube_lib INTERFACE)
target_sources(ledcube_lib INTERFACE /src/StepperMotor.c)
target_include_directories(ledcube_lib INTERFACE ${CMAKE_CURRENT_LIST_DIR})
#Link pico_stdlib to stepper_motor library
target_link_libraries(ledcube_lib INTERFACE pico_stdlib)

add_executable(${projname}
	main.c
	lib/led_cube/src/led_cube.c
	)

target_link_libraries(${projname}
	pico_stdlib
	pico_multicore
	hardware_i2c
	)

add_compile_options(-Wall
	-Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
	-Wno-unused-function # we have some for the docs that aren't called
	-Wno-maybe-uninitialized
	)

pico_enable_stdio_usb(${projname} 1)
pico_enable_stdio_uart(${projname} 0)

pico_add_extra_outputs(${projname})
