file(GLOB_RECURSE COM_YANKON_HAL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c )
set(yankon_hal_static_target "myHal")
add_library(${yankon_hal_static_target} STATIC ${COM_YANKON_HAL_SRC})
add_library(ln::hello ALIAS ${yankon_hal_static_target})
target_include_directories(${yankon_hal_static_target}
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)