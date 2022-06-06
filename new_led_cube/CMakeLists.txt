set(modulename "new_led_cube")
set(SOURCES 
        led_rgb.cpp
        include/led_rgb.hpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
