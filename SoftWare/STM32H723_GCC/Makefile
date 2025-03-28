##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [4.5.0-B34] date: [Mon Jan 20 10:38:27 CST 2025] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = STM32H723_GCC


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

# 根据操作系统自动选择可执行文件名
ifeq ($(OS),Windows_NT)  # 检测 Windows 系统‌:ml-citation{ref="1,3" data="citationList"}
    JLINKEXE := JLink.exe
    JLINKGDBSERVER := JLinkGDBServer.exe
else  # 默认处理 Linux/Unix 系统（如 Ubuntu 22.04）‌:ml-citation{ref="1,2" data="citationList"}
    JLINKEXE := JLinkExe
    JLINKGDBSERVER := JLinkGDBServerExe
endif

CHIP := STM32H723VG
GDB := arm-none-eabi-gdb.exe

ANGLE_SENSOR_INDEX = 1
#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/main.c \
Src/gpio.c \
Src/adc.c \
Src/stm32h7xx_it.c \
Src/stm32h7xx_hal_msp.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c \
Src/system_stm32h7xx.c \
Src/tim.c \
Src/i2c.c \
Src/usart.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c \
Src/dma.c \
Src/rng.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rng.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rng_ex.c \
Src/fdcan.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_fdcan.c 
# ASM sources
ASM_SOURCES =  \
startup_stm32h723xx.s

# ASM sources
ASMM_SOURCES = 


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m7

# fpu
FPU = -mfpu=fpv5-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32H723xx  \
-DSTM32H723xx
# AS includes
# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includes# AS includesAS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IInc \
-IDrivers/STM32H7xx_HAL_Driver/Inc \
-IDrivers/STM32H7xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32H7xx/Include \
-IDrivers/CMSIS/Include

 
C_SOURCES += $(wildcard ../Common/debuglog/SEGGER_RTT_V792h/RTT/*.c)
C_SOURCES += $(wildcard ../Common/debuglog/*.c)
C_INCLUDES += -I../Common/debuglog/SEGGER_RTT_V792h/Config 
C_INCLUDES += -I../Common/debuglog/SEGGER_RTT_V792h/RTT 
C_INCLUDES += -I../Common/debuglog 
# C_DEFS += -DDEBUG_MODE_UART

C_INCLUDES += -I../Common/systemmodule
C_SOURCES += $(wildcard ../Common/systemmodule/*.c)


C_INCLUDES += -I../Common/data_structures
C_SOURCES += $(wildcard ../Common/data_structures/heap.c)
C_SOURCES += $(wildcard ../Common/data_structures/fifo.c)


C_INCLUDES += -I../I2cBus 
C_SOURCES += $(wildcard ../I2cBus/*.c)

C_INCLUDES += -I../Hardware 
C_SOURCES += $(wildcard ../Hardware/tca9539pwr.c)
C_SOURCES += $(wildcard ../Hardware/ian226.c)
C_SOURCES += $(wildcard ../Hardware/hall_sensor.c)

C_INCLUDES += -I./board
C_SOURCES += $(wildcard ./board/board.c board_protocolInterface.c)
C_SOURCES += $(wildcard ./board/board_protocolInterface.c)

C_INCLUDES += -I../Protocol
C_SOURCES += $(wildcard ../Protocol/*.c)
C_INCLUDES += -I../ProtocolV3/uavcan/libcanard
C_SOURCES += $(wildcard ../ProtocolV3/uavcan/libcanard/*.c)
C_INCLUDES += -I../ProtocolV3/uavcan/dsdl/dinosaurs/actuator/wheel_motor
C_INCLUDES += -I../ProtocolV3/uavcan/dsdl

C_INCLUDES += -I../Common/method 
C_SOURCES += $(wildcard ../Common/method/trajectory_planning.c)
C_SOURCES += $(wildcard ../Common/method/random.c)
C_SOURCES += $(wildcard ../Common/method/pid.c)
C_SOURCES += $(wildcard ../Common/method/filter.c)
C_SOURCES += $(wildcard ../Common/method/datatypecovert.c)

C_INCLUDES += -I../Common/flash
C_SOURCES += $(wildcard ../Common/flash/*.c)
C_INCLUDES += -I../Common/Fsm 
C_INCLUDES += -I../BLDCMotor 
C_SOURCES += $(wildcard ../BLDCMotor/*.c)

CMBACKTRACE_ENABLE = 0
ifeq ($(CMBACKTRACE_ENABLE),1) 
C_DEFS += -DCMB_USER_CFG
C_SOURCES += $(wildcard ../Common/CmBacktrace/cm_backtrace/*.c)
C_INCLUDES += -I../Common/CmBacktrace/cm_backtrace 
ASM_SOURCES += $(wildcard ../Common/CmBacktrace/cm_backtrace/fault_handler/gcc/cmb_fault.s)
endif


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -error-function-declaration

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32H723VGTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
LDFLAGS += -lrdimon -u _printf_float

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)


dapdown:
	openocd -f cmsis-dap.cfg -f stm32h7x.cfg -c "init" -c "reset halt" -c \
	"flash write_image erase $(BUILD_DIR)/$(TARGET).hex" -c reset -c shutdown


stlinkdown:
	openocd -f stlink.cfg -f stm32h7x.cfg -c "init" -c "reset halt" -c \
	"flash write_image erase $(BUILD_DIR)/$(TARGET).hex" -c reset -c shutdown

jlinkdown:
	$(Q)echo "h" >> jlink.jlink
	$(Q)echo "loadfile" $(BUILD_DIR)/$(TARGET).hex >> jlink.jlink
	$(Q)echo "r" >> jlink.jlink             
	$(Q)echo "q" >> jlink.jlink   
	$(Q)$(JLINKEXE) -device $(CHIP) -Speed 4000 -IF SWD -CommanderScript jlink.jlink
# *** EOF ***
