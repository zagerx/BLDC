## 报错处理

`0004217 C Control block not found [__main__]`
- 找到.map文件，搜索`.bss._SEGGER_RTT`
- 取出对应的地址
- 输入`pyocd rtt -t STM32G473VETx -a 0x2000045c`