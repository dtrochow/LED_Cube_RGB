set(SOURCES 
        led_cube.c
        include/led_cube.h
)
add_library(led_cube ${SOURCES})
target_include_directories(led_cube PUBLIC include)
target_link_libraries(led_cube pico_stdlib mcp23017_lib)