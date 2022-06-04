set(SOURCES 
        led_cube.cpp
        led_animations.cpp
        include/led_cube.hpp
        include/led_animations_api.hpp
)
add_library(led_cube ${SOURCES})
target_include_directories(led_cube PUBLIC include)
target_link_libraries(led_cube pico_stdlib mcp23017_lib hardware_regs)