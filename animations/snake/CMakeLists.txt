set(modulename "snake_animation")
set(SOURCES 
        snake.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} cube hardware_regs animation_utils)
