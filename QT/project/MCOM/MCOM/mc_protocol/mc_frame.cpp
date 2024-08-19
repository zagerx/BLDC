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
  
std::vector<unsigned char> MC_Frame::UnPack() const {
    std::vector<unsigned char> frame;
  
    // 添加帧头
    frame.push_back((uint8_t)(FRAME_HEAD>>8));
    frame.push_back((uint8_t)(FRAME_HEAD));
  
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
    frame.push_back((uint8_t)(FRAME_TAIL>>8));
    frame.push_back((uint8_t)(FRAME_TAIL));
    return frame;
}

#include <iostream>
#include <vector>
#include <iomanip> // 用于 std::hex 和 std::setw
#include <QtDebug>

bool MC_Frame::Pack(const std::vector<unsigned char>& input) {
    // 验证输入长度是否足够（至少包含帧头、CMD、数据长度、CRC和帧尾）  
    if (input.size() < 10) { // 帧头(2) + CMD(2) + 数据长度(2) + CRC(2) + 帧尾(2)  
        return false;  
    }  

    // 验证帧头  
    if (input[0] != (FRAME_HEAD >> 8) || input[1] != (FRAME_HEAD & 0xFF)) {  
        return false;  
    }  
  
    // 读取CMD  
    CMD = (input[2] << 8) | input[3];
  
    // 读取数据长度  
    uint16_t dataLength = (input[4] << 8) | input[5];  
  
    // 验证总长度（包括帧头、CMD、数据长度、数据、CRC和帧尾）  
    if (input.size() != 10 + dataLength) {  
        return false;  
    }  
  
    // 读取数据  
    data.clear(); // 清空frame的data成员
    for (size_t i = 0; i < dataLength; ++i) {  
        data.push_back(input[6 + i]);
    }
    // qDebug() << "ssssss";
    // for (auto byte : data) {
    //     // 打印每个字节，以十六进制形式，前面补0以保持两位宽度
    //     std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << ' ';
    // }
    // 验证CRC（不包括帧尾）  
    // uint16_t crc = CalculateCRC16(input.data(), input.size() - 2); // 预留2字节给帧尾
    // uint16_t expectedCRC = (input[input.size() - 2] << 8) | input[input.size() - 1];
    // if (crc != expectedCRC) {
    //     return false;
    // }

    // 验证帧尾（这一步实际上是多余的，但保留以保持一致性）  
    if (input[input.size() - 2] != (FRAME_TAIL >> 8) || input[input.size() - 1] != (FRAME_TAIL & 0xFF)) {  
        return false;  
    }  
  
    // 如果所有检查都通过，则返回true  
    return true;  
}  

void MC_Frame::PrintFrame() const {  
    // 使用qDebug()打印CMD成员  
    qDebug() << "cur cmd = " << QString::number(CMD, 16).toUpper().rightJustified(2, '0');  
  
    // 使用qDebug()遍历并打印data成员中的每个字节  
    for (unsigned char byte : data) {  
        qDebug().nospace() << QString::number(byte, 16).toUpper().rightJustified(2, '0') << " ";  
    }    
}
