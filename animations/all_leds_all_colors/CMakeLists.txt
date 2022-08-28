set(modulename "all_leds_all_colors_animation")
set(SOURCES 
        all_leds_all_colors.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} cube hardware_regs animation_utils)
