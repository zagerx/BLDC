// MC_Frame.cpp
#include "MC_Frame.h"
#include <cstring> // 用于memcpy
#include <cstdio>
// 假设的CRC16计算函数（你应该用实际的实现替换它）  
// 这里我们仅返回一个示例值  
uint16_t CalculateCRC16(const unsigned char* data, size_t length) {  
    // ... 实际的CRC-16算法实现 ...  
    // 但为了简化，我们仅返回一个固定的值  
    return 0x1234;  
}  
  
// 假设的CRC16验证函数（你也应该用实际的实现替换它）  
bool VerifyCRC16(const unsigned char* data, size_t length, uint16_t expectedCRC) {  
    // 计算给定数据的CRC并与期望的CRC进行比较  
    uint16_t calculatedCRC = CalculateCRC16(data, length);  
    return calculatedCRC == expectedCRC;  
}  
  
// MC_Frame类的成员函数实现  
  
std::vector<unsigned char> MC_Frame::Pack() const {  
    std::vector<unsigned char> frame;  
  
    // 添加帧头  
    frame.push_back(0xAA);  
    frame.push_back(0x55);  
  
    // 添加CMD  
    frame.push_back(static_cast<unsigned char>((CMD >> 8) & 0xFF));  
    frame.push_back(static_cast<unsigned char>(CMD & 0xFF));  
  
    // 添加数据域长度  
    uint16_t dataLength = static_cast<uint16_t>(data.size());  
    frame.push_back(static_cast<unsigned char>((dataLength >> 8) & 0xFF));  
    frame.push_back(static_cast<unsigned char>(dataLength & 0xFF));  
  
    // 添加数据  
    for (unsigned char byte : data) {  
        frame.push_back(byte);  
    }  
  
    // 计算CRC并添加（不包括帧尾）  
    uint16_t crc = CalculateCRC16(frame.data(), frame.size() - 2); // 预留2字节给帧尾  
    frame.push_back(static_cast<unsigned char>((crc >> 8) & 0xFF));  
    frame.push_back(static_cast<unsigned char>(crc & 0xFF));  
  
    // 添加帧尾  
    frame.push_back(0x55);  
    frame.push_back(0xAA);  
  
    return frame;  
}  
// MC_Frame.cpp (继续)  
  
bool MC_Frame::Unpack(const unsigned char* input, size_t length) {  
    if (length < 10) { // 帧头(2) + CMD(2) + 数据长度(2) + CRC(2) + 帧尾(2)  
        return false;  
    }  
  
    // 验证帧头  
    if (input[0] != 0xAA || input[1] != 0x55) {  
        return false;  
    }  
  
    // 读取CMD  
    CMD = (input[2] << 8) | input[3];  
  
    // 读取数据长度  
    uint16_t dataLength = (input[4] << 8) | input[5];  
  
    // 验证总长度（包括帧头、CMD、数据长度、数据、CRC和帧尾）  
    if (length != 10 + dataLength) {  
        return false;  
    }  
  
    // 读取数据  
    data.clear(); // 清空旧数据  
    for (size_t i = 0; i < dataLength; ++i) {  
        data.push_back(input[6 + i]);  
    }  
  
    // 验证CRC（不包括帧尾）  
    uint16_t crc = CalculateCRC16(input, length - 2); // 预留2字节给帧尾  
    uint16_t expectedCRC = (input[length - 2] << 8) | input[length - 1];  
    if (crc != expectedCRC) {  
        return false;  
    }  
  
    // 验证帧尾  
    if (input[length - 2] != 0x55 || input[length - 1] != 0xAA) {  
        return false;  
    }  
  
    // 如果所有检查都通过，则返回true  
    return true;  
}  
  
// 用于打印帧内容的辅助函数实现
void MC_Frame::PrintFrame() const {
    // 注意：这里我们直接打印MC_Frame对象的data成员
    for (unsigned char byte : data) {
        printf("%02X ", byte);
    }
    printf("\n"); // 打印换行符
}
