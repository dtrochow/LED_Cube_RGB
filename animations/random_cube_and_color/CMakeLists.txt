set(modulename "random_cube_and_color_animation")
set(SOURCES 
        random_cube_and_color.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} cube hardware_regs animation_utils)
