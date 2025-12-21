cmake_minimum_required(VERSION 3.13)


target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    _current_calib.c
    _pp_ident.c
    _encoder_calib.c
    motor_carible.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)