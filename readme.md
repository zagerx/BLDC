# 版本说明
## v1.0.0
- H7工程增加下功能
    - debuglog模块
    - I2cBus模块
    - INA226 TCA9539
# template_proj_branch说明
- 该分支用作测试`芯片功能及板子外设`
- 测试公共模块

## 添加测试文件
```
C_SOURCES += $(wildcard ../Hardware/ian226.c)
C_SOURCES += $(wildcard ../Hardware/at24c0x.c)
C_SOURCES += $(wildcard ../Hardware/tca9539pwr.c)
C_INCLUDES += -I../Hardware 

C_SOURCES += $(wildcard ../I2cBus/*.c)
C_INCLUDES += -I../I2cBus 
```

