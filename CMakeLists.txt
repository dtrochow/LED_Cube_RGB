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

# Directories for mcp23017 library
include_directories(lib/mcp23017)
link_directories(lib/mcp23017)

# Directories for led_cube
include_directories(led_cube)
link_directories(led_cuve)

add_executable(${projname}
	main.c
	)

target_link_libraries(${projname}
	pico_stdlib
	pico_multicore
	hardware_i2c
	mcp23017_lib
	led_cube
	)

add_compile_options(-Wall
	-Wformat-overflow
	-Wbad-function-cast
	-Wcast-qual
	-Werror
	-Wpedantic
	-Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
	-Wno-unused-function # we have some for the docs that aren't called
	-Wno-maybe-uninitialized
	-Wpadded
	)

pico_enable_stdio_usb(${projname} 0)
pico_enable_stdio_uart(${projname} 1)

pico_add_extra_outputs(${projname})
