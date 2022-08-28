set(modulename "animation_utils")
set(SOURCES 
        animation_utils.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} cube hardware_regs)
