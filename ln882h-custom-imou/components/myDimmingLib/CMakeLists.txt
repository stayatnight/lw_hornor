file(GLOB_RECURSE COM_DIM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c )
set(dimming_static_target "myDimming")
add_library(${dimming_static_target} STATIC ${COM_DIM_SRC})
add_library(ln::hello ALIAS ${dimming_static_target})
target_include_directories(${dimming_static_target}
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)