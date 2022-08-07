set(modulename "animations")
set(SOURCES 
        animations.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} cube)
target_link_libraries(${modulename} hardware_regs)
