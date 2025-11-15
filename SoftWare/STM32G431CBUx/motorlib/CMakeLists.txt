cmake_minimum_required(VERSION 3.13)

add_subdirectory(bsp)

file(GLOB _driver_src CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/driver/*.c")
file(GLOB _foc_src CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/foc/*.c")
file(GLOB _sm_src CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/statemachine/*.c")

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    motor.c
    motor_cfg.c
    motor_mode.c
    motor_state.c
    ${_driver_src}
    ${_foc_src}
    ${_sm_src}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/driver
    ${CMAKE_CURRENT_SOURCE_DIR}/foc
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine
)

