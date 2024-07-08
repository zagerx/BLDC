#include "protocol.h"
#include <cstring> // 用于memcpy等函数
unsigned short calculateCRC16(unsigned short head, unsigned short cmd, unsigned short length, const unsigned char *data, unsigned short dataLength);

protocol::protocol() {
    // 初始化frame成员，例如可以设置head和tail为特定的值
    frame.head = 0xAA55; // 示例值
    frame.tail = 0x55AA; // 示例值
    frame.data = nullptr; // 初始时data不指向任何内存
}

protocol::~protocol() {
    // 释放动态分配的内存
    if (frame.data != nullptr) {
        delete[] frame.data;
        frame.data = nullptr;
    }
}

bool protocol::pack(unsigned short cmd, const unsigned char *data, unsigned short dataLength, frame_t &frame) {
    // 设置frame的各个字段
    frame.CMD = cmd;
    frame.length = dataLength;
    frame.data = new unsigned char[dataLength]; // 动态分配内存
    if (frame.data == nullptr) {
        return false; // 内存分配失败
    }
    std::memcpy(frame.data, data, dataLength); // 复制数据
    // 计算CRC16，这里假设有一个计算CRC16的函数
    frame.crc16 = calculateCRC16(frame.head, frame.CMD, frame.length, frame.data, dataLength);
    frame.tail = 0x55AA; // 设置tail字段
    return true;
}

bool protocol::unpack(const frame_t &frame, unsigned short &cmd, unsigned char *&data, unsigned short &dataLength) {
    // 从frame中提取数据
    cmd = frame.CMD;
    dataLength = frame.length;
    data = new unsigned char[dataLength]; // 动态分配内存
    if (data == nullptr) {
        return false; // 内存分配失败
    }
    std::memcpy(data, frame.data, dataLength); // 复制数据
    // 可以在这里进行CRC校验
    return true;
}

// 假设的CRC16计算函数
unsigned short calculateCRC16(unsigned short head, unsigned short cmd, unsigned short length, const unsigned char *data, unsigned short dataLength) {
    // 这里应该是CRC16的计算代码，这里只是一个占位符
    return 0; // 返回实际的CRC16值
}
