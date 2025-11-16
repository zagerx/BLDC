cmake_minimum_required(VERSION 3.13)

# target_sources(${PROJECT_NAME} stm32_pwm_eulr.c)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    stm32_pwm_eulr.c
    stm32_as5047.c
)