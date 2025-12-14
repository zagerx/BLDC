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

# 将隐式函数声明警告视为错误
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE 
    -Werror=implicit-function-declaration
)
# =========================================
# Add motorlib module
# =========================================
add_subdirectory(motorlib)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/motorlib  
)
# =========================================
# Add hmt_log module
# =========================================
add_subdirectory(hmt_log)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/hmt_log
)


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
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wl,-u,_printf_float
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

# ==================================================
# 添加 JLink 下载目标（兼容多配置构建）
# ==================================================
# MCU 型号（用于 JLink 下载）
set(CHIP "STM32G431CB")

# ==================================================
# 必须指定 chip 型号，否则 JLink 下载无法工作
# ==================================================
if(NOT DEFINED CHIP)
    message(FATAL_ERROR 
        "CHIP is not defined!\n"
        "Please specify the MCU model used by JLink.\n"
        "For example:\n"
        "    set(CHIP \"STM32G431CB\")\n"
    )
endif()

# 寻找 JLink 工具（兼容 Windows / Linux）
if(WIN32)
    find_program(JLINKEXE NAMES JLink.exe)
else()
    find_program(JLINKEXE NAMES JLinkExe)
endif()

if(NOT JLINKEXE)
    message(WARNING "JLink not found, JLink-based download target will NOT be available")
else()
    message(STATUS "Found JLink: ${JLINKEXE}")

    # 生成 jlink.jlink 脚本
    set(JLINK_SCRIPT ${CMAKE_BINARY_DIR}/jlink.jlink)

    add_custom_command(
        OUTPUT ${JLINK_SCRIPT}
        COMMAND ${CMAKE_COMMAND} -E echo "h" > ${JLINK_SCRIPT}
        COMMAND ${CMAKE_COMMAND} -E echo "loadfile $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}${OUTPUT_SUFFIX}.hex" >> ${JLINK_SCRIPT}
        COMMAND ${CMAKE_COMMAND} -E echo "r" >> ${JLINK_SCRIPT}
        COMMAND ${CMAKE_COMMAND} -E echo "q" >> ${JLINK_SCRIPT}
        COMMENT "Generating JLink download script"
    )

    # jlink_down 目标（CMake 多配置构建下可用）
    add_custom_target(jlink_down
        COMMAND ${JLINKEXE}
                -device ${CHIP}
                -Speed 4000
                -IF SWD
                -CommanderScript ${JLINK_SCRIPT}
        DEPENDS ${CMAKE_PROJECT_NAME} ${JLINK_SCRIPT}
        COMMENT "Downloading ${CMAKE_PROJECT_NAME}${OUTPUT_SUFFIX}.hex using JLink"
        USES_TERMINAL
    )
endif()

