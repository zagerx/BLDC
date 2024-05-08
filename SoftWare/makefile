# 默认目标
all: target1 target2

# 编译 target1
target1:
    @$(MAKE) -C STM32F405RG_GCC -f ../Makefile

# 编译 target2
target2:
    @$(MAKE) -C STM32G431_GCC -f ../Makefile

# 清理目标
clean:
    @$(MAKE) -C STM32F405RG_GCC -f ../Makefile clean
    @$(MAKE) -C STM32G431_GCC -f ../Makefile clean
