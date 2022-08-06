cmake_minimum_required(VERSION 3.14)
set(projname "led_cube_rgb_unit")
project(${projname})

macro(add_utest test_name test_files dependencies)
  add_executable(${test_name}
    ${test_files}
  )
  target_link_libraries(${test_name}
    ${dependencies}
    gtest_main
    gmock_main
  )
  add_test(${test_name} ${test_name})
endmacro()


# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../led_cube/led_matrix ${CMAKE_CURRENT_BINARY_DIR}/led_matirx)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../led_cube/led_rgb ${CMAKE_CURRENT_BINARY_DIR}/led_rgb)

set (deps
  led_matrix
  led_rgb
)

add_utest("led_rgb_test" led_rgb_test.cpp ${deps})
add_utest("led_matrix_test" led_matrix_test.cpp ${deps})
