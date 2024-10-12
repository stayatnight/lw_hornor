file(GLOB_RECURSE COM_CRC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c )
set(mycrc_static_target "myCrc")
add_library(${mycrc_static_target} STATIC ${COM_CRC_SRC})
add_library(ln::hello ALIAS ${mycrc_static_target})
target_include_directories(${mycrc_static_target}
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)