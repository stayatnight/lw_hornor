file(GLOB_RECURSE COM_KEY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c )
set(key_static_target "myKey")
add_library(${key_static_target} STATIC ${COM_KEY_SRC})
add_library(ln::hello ALIAS ${key_static_target})
target_include_directories(${key_static_target}
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)