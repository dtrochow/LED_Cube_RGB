set(modulename "cube")
set(SOURCES 
        led_cube.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} led_matrix led_rgb memory_hub)
