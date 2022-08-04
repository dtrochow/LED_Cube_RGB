set(modulename "led_rgb")
set(SOURCES 
        led_rgb.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC mocks)
target_include_directories(${modulename} PUBLIC include)
