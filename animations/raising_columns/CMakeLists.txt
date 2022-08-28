set(modulename "raising_columns_animation")
set(SOURCES 
        raising_columns.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} cube hardware_regs animation_utils)
