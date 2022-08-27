add_subdirectory(animation_utils)

set(modulename "animations")

set(animations_modules
    animation_utils
)

add_library(${modulename} INTERFACE)
target_include_directories(${modulename} INTERFACE include)
target_link_libraries(${modulename} INTERFACE ${animations_modules})
