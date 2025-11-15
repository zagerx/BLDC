cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32G431CBUx)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)


# =========================================
# Add motorlib module
# =========================================
add_subdirectory(motorlib)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/motorlib  
)
# target_sources(${CMAKE_PROJECT_NAME} PRIVATE
#     ${MOTORLIB_SOURCES}
# )

# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
#     ${MOTORLIB_INCLUDES}
# )


# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
# 生成HEX和BIN文件的配置
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OUTPUT_SUFFIX "_debug")
else()
    set(OUTPUT_SUFFIX "")
endif()

# 生成HEX文件
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex 
    "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>" 
    "${CMAKE_PROJECT_NAME}${OUTPUT_SUFFIX}.hex"
    COMMENT "Building ${CMAKE_PROJECT_NAME}${OUTPUT_SUFFIX}.hex"
)

# 生成BIN文件
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary 
    "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>" 
    "${CMAKE_PROJECT_NAME}${OUTPUT_SUFFIX}.bin"
    COMMENT "Building ${CMAKE_PROJECT_NAME}${OUTPUT_SUFFIX}.bin"
)

# 显示固件大小信息
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
    COMMENT "Firmware size:"
)



# ==================================================
# 添加下载目标
# ==================================================

# 查找OpenOCD
find_program(OPENOCD_EXECUTABLE openocd)
if(NOT OPENOCD_EXECUTABLE)
    message(WARNING "OpenOCD not found, download targets will not be available")
else()
    message(STATUS "Found OpenOCD: ${OPENOCD_EXECUTABLE}")
    
    # 下载目标 - 使用HEX文件
    add_custom_target(down
        COMMAND ${OPENOCD_EXECUTABLE} -f ${CMAKE_SOURCE_DIR}/stlink.cfg 
                -f ${CMAKE_SOURCE_DIR}/stm32g4x.cfg 
                -c "init" 
                -c "reset halt" 
                -c "flash write_image erase $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}${OUTPUT_SUFFIX}.hex" 
                -c "reset" 
                -c "shutdown"
        DEPENDS ${CMAKE_PROJECT_NAME}
        COMMENT "Downloading ${CMAKE_PROJECT_NAME}${OUTPUT_SUFFIX}.hex to STM32 via ST-Link"
        USES_TERMINAL
    )
endif()    