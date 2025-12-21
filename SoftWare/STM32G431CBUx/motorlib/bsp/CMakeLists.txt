cmake_minimum_required(VERSION 3.13)

set(MODE_SRCS "")
if(MOTOR_TYPE_HMT)
    list(APPEND MODE_SRCS
        stm32_pwm.c
        stm32_mt6816.c
    )
else()
    list(APPEND MODE_SRCS
        stm32_pwm_eulr.c
        stm32_as5047.c
    )
endif()
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MODE_SRCS})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
