set(modulename "cube")
set(SOURCES 
        led_cube.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} led_matrix led_rgb memory_hub)

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