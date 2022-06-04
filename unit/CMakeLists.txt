cmake_minimum_required(VERSION 3.14)
set(projname "led_cube_rgb_unit")
project(${projname})

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../new_led_cube ${CMAKE_CURRENT_BINARY_DIR}/new_led_cube)

add_executable(${projname}
  3d_matrix_test.cpp
  animations_test.cpp
  led_rgb_test.cpp
)

target_link_libraries(${projname}
  new_led_cube
  gtest_main
)

add_test(${projname} ${projname})
