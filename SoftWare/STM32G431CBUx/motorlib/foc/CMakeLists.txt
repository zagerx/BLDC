cmake_minimum_required(VERSION 3.13)


target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    foc_data.c
    foc.c
    foc_pid.c
    foc.c
    openloop_voltage.c
    svpwm.c
    t_trajectory.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)