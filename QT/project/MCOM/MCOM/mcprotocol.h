#ifndef MCPROTOCOL_H
#define MCPROTOCOL_H
#include <vector>  
#include <iostream>  
#include <cassert> 
class CircularBuffer {  
public:  
    CircularBuffer(size_t size) : buffer(size), head(0), tail(0), full(false) {  
        assert(size > 0);  
    }  
  
    bool push(unsigned char data) {  
        if (full) return false;  
        buffer[tail] = data;  
        tail = (tail + 1) % buffer.size();  
        if (tail == head) {  
            full = true;  
        }  
        return true;  
    }  
  
    bool pop(unsigned char& data) {  
        if (head == tail && !full) return false;  
        data = buffer[head];  
        head = (head + 1) % buffer.size();  
        full = false;  
        return true;  
    }  
  
    bool is_full() const { return full; }  
    bool is_empty() const { return head == tail && !full; }  
  
private:  
    std::vector<unsigned char> buffer;  
    size_t head, tail;  
    bool full;
};
class McProtocol
{
public:
    McProtocol(size_t send_buffer_size)
        : send_buffer(send_buffer_size){}
  
    ~McProtocol(); // 添加析构函数以确保动态分配的内存得到释放

    bool send_data(unsigned char *data,unsigned short len);
    bool recive_data(unsigned char data);
    bool process_data(void);
private:
    CircularBuffer send_buffer;  
};

#endif // MCPROTOCOL_H
