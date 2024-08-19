#include "mc_protocol.h"
#include <stdexcept> // 用于抛出异常
#include <QtDebug>
#include "comment.h"

void MCProtocol::AddFrameToBuf(const MC_Frame& frame) {  
    if (currentSendBufferSize + frame.UnPack().size() > bufferSize) {  
        // 处理缓冲区溢出（例如：抛出异常、记录错误、丢弃旧数据等）
        throw std::runtime_error("Send buffer overflow");
    }

    std::vector<unsigned char> frameBytes = frame.UnPack();
    sendBuffer.push(frameBytes);
    // PrintBytes(frameBytes);
    currentSendBufferSize += frameBytes.size();  
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
    currentSendBufferSize -= data.size();
    return data;  
}  
  
void MCProtocol::ClearSendBuffer() {  
    while (!sendBuffer.empty()) {  
        sendBuffer.pop();  
    }  
    currentSendBufferSize = 0;  
}

void MCProtocol::ReceiveData(const std::vector<unsigned char>& data) {
    if (currentRecvBufferSize + data.size() > bufferSize) {
        return;
        // 处理接收缓冲区溢出
        throw std::runtime_error("Receive buffer overflow");
    }
    // PrintBytes(data);
    recvQueue.push(data);
    currentRecvBufferSize += data.size();  
}


bool MCProtocol::RDFrameFromRecvBuf(MC_Frame *revframe) {
    if (recvQueue.empty()) {  
        // 如果缓冲区为空，则无法读取帧  
        // throw std::runtime_error("No data in receive buffer");
        return false;
    }
    std::vector<unsigned char> frameData = recvQueue.front();
    recvQueue.pop();
    currentRecvBufferSize -= frameData.size();
    revframe->Pack(frameData);
    return true;
}



