#ifndef MC_PROTOCOL_H  
#define MC_PROTOCOL_H  
  
#include <vector>
#include <queue>  // 添加这一行以包含 queue 头文件

#include "mc_frame.h"
  
class MCProtocol {  
private:  
    std::queue<std::vector<unsigned char>> sendBuffer;  
    std::queue<std::vector<unsigned char>> recvQueue;  // 新增接收缓冲区

    size_t bufferSize;
    size_t currentSendBufferSize;
    size_t currentRecvBufferSize;  // 跟踪接收缓冲区当前大小
  
public:
    MCProtocol(size_t size) : bufferSize(size), currentSendBufferSize(0) {}  
  
    // 将MC_Frame对象封包后发送到缓冲区  
    void AddFrameToBuf(const MC_Frame& frame);  
  
    // 检查发送缓冲区是否已满
    bool SendbufHasData() const;
  
    // 从发送缓冲区读取一个完整的帧（注意：这里不直接返回MC_Frame，而是返回字节数组）  
    // 调用者需要自行处理字节数组以构造MC_Frame对象  
    std::vector<unsigned char> RDFromSendBuf();  
  
    // 可选：清空发送缓冲区  
    void ClearSendBuffer();  

    // 接收数据到接收缓冲区  
    void ReceiveData(const std::vector<unsigned char>& data);  
  
    // 尝试从接收缓冲区读取并解析一个完整的MC_Frame
    bool RDFrameFromRecvBuf(MC_Frame *revframe);

};  

#endif // MC_PROTOCOL_H
