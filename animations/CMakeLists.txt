add_subdirectory(rain)
add_subdirectory(random_cube_and_color)
add_subdirectory(all_leds_all_colors)
add_subdirectory(animation_utils)

set(modulename "animations")

set(animations_modules
	rain_animation
	random_cube_and_color_animation
	all_leds_all_colors_animation
    animation_utils
)

add_library(${modulename} INTERFACE)
target_include_directories(${modulename} INTERFACE include)
target_link_libraries(${modulename} INTERFACE ${animations_modules})
