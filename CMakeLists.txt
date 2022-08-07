cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

set(projname "led_cube_rgb")

project(${projname} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

pico_sdk_init()

add_subdirectory(lib)
add_subdirectory(led_cube)

add_executable(${projname}
	main.cpp
	)

target_link_libraries(${projname}
	pico_stdlib
	pico_multicore
	hardware_i2c
	mcp23017_lib
	cube
	animations
	)

pico_enable_stdio_usb(${projname} 0)
pico_enable_stdio_uart(${projname} 1)

pico_add_extra_outputs(${projname})
