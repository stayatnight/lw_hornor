file(GLOB_RECURSE COM_YANKON_HAL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c )
set(yankon_hal_static_target "myHal")
add_library(${yankon_hal_static_target} STATIC ${COM_YANKON_HAL_SRC})
add_library(ln::hello ALIAS ${yankon_hal_static_target})
target_include_directories(${yankon_hal_static_target}
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS_Adapter)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS/Source/include)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F)
include_directories(
    ${CMAKE_SOURCE_DIR}/project/${USER_PROJECT}/bsp
    ${CMAKE_SOURCE_DIR}/project/${USER_PROJECT}/cfg
    ${MCU_LN882X_DIR}/ln882h
    ${MCU_LN882X_DIR}/CMSIS_5.3.0
    ${MCU_LN882X_DIR}/driver_ln882x/inc
    ${MCU_LN882X_DIR}/driver_ln882x/inc/ll
    ${COMP_KERNEL_DIR}
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/include
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${COMP_WIFI_DIR}
    ${CMAKE_SOURCE_DIR}/components
    ${COMP_LWIP_DIR}/include
    ${COMP_LWIP_DIR}/port
    ${MCU_LN882X_DIR}/ln88xx
    D:/code/h_test/honor_magiclink_sdk/ln882h-custom-imou/project/yankon/inc
)