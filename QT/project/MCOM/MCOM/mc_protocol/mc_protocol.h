#ifndef MC_PROTOCOL_H  
#define MC_PROTOCOL_H  
  
#include <vector>  
#include <queue>  // 添加这一行以包含 queue 头文件  

#include "MC_Frame.h"  
  
class MCProtocol {  
private:  
    std::queue<std::vector<unsigned char>> sendBuffer;  
    size_t bufferSize;  
    size_t currentBufferSize;  
  
public:  
    MCProtocol(size_t size) : bufferSize(size), currentBufferSize(0) {}  
  
    // 将MC_Frame对象封包后发送到缓冲区  
    void SendFrame(const MC_Frame& frame);  
  
    // 检查发送缓冲区是否已满  
    bool SendbufIsFull() const;  
  
    // 从发送缓冲区读取一个完整的帧（注意：这里不直接返回MC_Frame，而是返回字节数组）  
    // 调用者需要自行处理字节数组以构造MC_Frame对象  
    std::vector<unsigned char> RDFromSendBuf();  
  
    // 可选：清空发送缓冲区  
    void ClearSendBuffer();  
};  
  
#endif // MC_PROTOCOL_H