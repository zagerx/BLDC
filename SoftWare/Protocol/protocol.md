## 帧格式
|帧头|命令|数据长度|数据0|...|数据n|CRC16|帧尾|
|-|-|-|-|-|-|-|-|
|0xA5A5|uint16_t|uint16_t|data1|...|datan|uint16_t|0x5A5A|

## 常用命令
|命令|高字节|低字节|功能|
|-|-|-|-|
||0x01 (主机下发)|||
|0x0101||0x01|设置速度|
|0x0102||0x02|设置位置|
|0x0103||0x03|电机失能|
|0x0104||0x04|电机使能|
|0x0105||0x05|电机正常模式|
|0x0106||0x06|电机调试模式|
|0x0107||0x07|获取电机信息|
|0x0108||0x08|获取PCBA信息|
|0x010A||0x0A|设置pid参数|
|0x0109||0x09||
||0x02 (从机上报)||
|0x0200||0x00|心跳|
|0x0201||0x01|上报当前速度/里程|
|`~0x0107`||0x07|响应电机信息|
|`~0x010F`||0x08|PCBA信息|
### 0x010A
    主机下发设置PID参数指令，数据格式如下:
|帧头|命令|数据长度|kp||||ki||||kc||||CRC16|帧尾|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
|0xA5A5|0x010A|12|byte3|byte2|byte1|byte0|byte3|byte2|byte1|byte0|byte3|byte2|byte1|byte0|uint16_t|0x5A5A|
## 协议简介

上位机MCOM和G431电机控制板通信协议，方便电机调试。


