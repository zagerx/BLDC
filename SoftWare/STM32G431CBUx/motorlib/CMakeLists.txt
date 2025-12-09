cmake_minimum_required(VERSION 3.13)

option(MOTOR_TYPE_HMT "使用华马台电机" OFF)
option(MOTOR_TYPE_EULRE "使用欧拉2802电机" ON)
option(DRIVER_TYPE_HMT "华马台驱动器" OFF)
option(DRIVER_TYPE_EULRE "欧拉驱动器" OFF)

add_subdirectory(bsp)
add_subdirectory(carible)

file(GLOB _driver_src CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/driver/*.c")
file(GLOB _foc_src CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/foc/*.c")
file(GLOB _sm_src CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/statemachine/*.c")

set(MODE_SRCS "")
if(MOTOR_TYPE_HMT)
    list(APPEND MODE_SRCS
        motor_cfg_hmt.c
    )
else()
    list(APPEND MODE_SRCS
        motor_cfg.c
    )
endif()
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MODE_SRCS})

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    motor.c
    motor_state.c
    ${_driver_src}
    ${_foc_src}
    ${_sm_src}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/carible
    ${CMAKE_CURRENT_SOURCE_DIR}/driver
    ${CMAKE_CURRENT_SOURCE_DIR}/foc
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine
)

