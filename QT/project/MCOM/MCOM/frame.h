#ifndef FRAME_H
#define FRAME_H


typedef struct _frame
{
    unsigned short head;
    unsigned short CMD;
    unsigned short length;
    unsigned char  *data;
    unsigned short crc16;
    unsigned short tail;
}frame_t;


class Frame
{
public:
    Frame();
    ~Frame();
    // 封包函数
    bool pack(unsigned short cmd, const unsigned char *data, unsigned short dataLength, frame_t &frame);

    // 解包函数
    bool unpack(const frame_t &frame, unsigned short &cmd, unsigned char *&data, unsigned short &dataLength);

private:
    frame_t frame; // 添加frame_t类型的成员
};

#endif // FRAME_H
