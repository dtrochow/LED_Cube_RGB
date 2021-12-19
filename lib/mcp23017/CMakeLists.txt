set(SOURCES 
        mcp23017.c
        include/mcp23017.h
)
add_library(mcp23017_lib ${SOURCES})
target_include_directories(mcp23017_lib PUBLIC include)
target_link_libraries(mcp23017_lib pico_stdlib hardware_i2c)