#include "mc_protocol.h"
#include <stdexcept> // 用于抛出异常
#include <QtDebug>


#include <iostream>
#include <vector>
#include <iomanip> // 用于 std::hex 和 std::setw
void PrintBytes(const std::vector<unsigned char>& bytes) {
    for (auto byte : bytes) {
        // 打印每个字节，以十六进制形式，前面补0以保持两位宽度
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << ' ';
    }
    std::cout << std::endl; // 打印完毕后换行
}


void MCProtocol::SendFrame(const MC_Frame& frame) {  
    if (currentBufferSize + frame.Pack().size() > bufferSize) {  
        // 处理缓冲区溢出（例如：抛出异常、记录错误、丢弃旧数据等）
        throw std::runtime_error("Send buffer overflow");
    }

    std::vector<unsigned char> frameBytes = frame.Pack();
    sendBuffer.push(frameBytes);
    // PrintBytes(frameBytes);
    currentBufferSize += frameBytes.size();  
}

bool MCProtocol::SendbufHasData() const {
    // 检查sendBuffer是否非空
    return !sendBuffer.empty();
}

std::vector<unsigned char> MCProtocol::RDFromSendBuf() {  
    if (sendBuffer.empty()) {  
        // 如果缓冲区为空，返回一个空的字节数组
        return {};
    }
    std::vector<unsigned char> data = sendBuffer.front();  
    sendBuffer.pop();  
    currentBufferSize -= data.size();
    // PrintBytes(data);

    return data;  
}  
  
void MCProtocol::ClearSendBuffer() {  
    while (!sendBuffer.empty()) {  
        sendBuffer.pop();  
    }  
    currentBufferSize = 0;  
}
