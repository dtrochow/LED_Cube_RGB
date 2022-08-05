set(modulename "led_matrix")
set(SOURCES 
        led_matrix.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} led_rgb)
