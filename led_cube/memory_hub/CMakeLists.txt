set(modulename "memory_hub")
set(SOURCES 
        memory_hub.cpp
)
add_library(${modulename} ${SOURCES})
target_include_directories(${modulename} PUBLIC include)
target_link_libraries(${modulename} pico_stdlib mcp23017_lib hardware_regs led_matrix)
