set(modulename "animation_runner")
set(SOURCES 
        animation_runner.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} cube animations)
