#include "mc_protocol.h"
#include <stdexcept> // 用于抛出异常
#include <QtDebug>

void MCProtocol::SendFrame(const MC_Frame& frame) {  
    if (currentBufferSize + frame.Pack().size() > bufferSize) {  
        // 处理缓冲区溢出（例如：抛出异常、记录错误、丢弃旧数据等）
        throw std::runtime_error("Send buffer overflow");
    }
    qDebug() << "SendFrame";

    std::vector<unsigned char> frameBytes = frame.Pack();  
    sendBuffer.push(frameBytes);  
    currentBufferSize += frameBytes.size();  
}

bool MCProtocol::SendbufIsFull() const {  
    return currentBufferSize >= bufferSize;  
}  
  
std::vector<unsigned char> MCProtocol::RDFromSendBuf() {  
    if (sendBuffer.empty()) {  
        // 如果缓冲区为空，返回一个空的字节数组  
        return {};  
    }

    std::vector<unsigned char> data = sendBuffer.front();  
    sendBuffer.pop();  
    currentBufferSize -= data.size();  
    return data;  
}  
  
void MCProtocol::ClearSendBuffer() {  
    while (!sendBuffer.empty()) {  
        sendBuffer.pop();  
    }  
    currentBufferSize = 0;  
}
