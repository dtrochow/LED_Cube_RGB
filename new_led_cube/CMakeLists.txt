set(modulename "new_led_cube")
set(SOURCES 
        3d_matrix.cpp
        animations.cpp
        led_rgb.cpp
        include/3d_matrix.hpp
        include/animations.hpp
        include/led_rgb.hpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
