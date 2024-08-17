// MC_Frame.h
#ifndef MC_FRAME_H
#define MC_FRAME_H

#include <vector>
#include <cstdint>


#define FRAME_HEAD       (0xA5A5)
#define FRAME_TAIL       (0x5A5A)
class MC_Frame {
public:
    uint16_t CMD;
    std::vector<unsigned char> data;

    // 解包函数声明
    std::vector<unsigned char> UnPack() const;

    // 封包函数声明
    bool Pack(const std::vector<unsigned char>& input);
    // 可选：用于打印帧内容的辅助函数声明
    void PrintFrame(void) const;

    // 注意：CRC16的计算和验证函数通常不会作为MC_Frame类的成员函数，
    // 但为了完整性，我们可以将它们声明为友元函数或在外部实现并在需要时调用。
    // 这里我们假设它们是在外部实现的。
};

#endif // MC_FRAME_H
