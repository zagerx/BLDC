#include "mcprotocol.h"
#include "stdbool.h"
McProtocol::~McProtocol() {

}
bool McProtocol::send_data(unsigned char *data,unsigned short len)
{  
    for (unsigned short i = 0; i < len; ++i) {  
        if (!send_buffer.push(data[i])) {  
            return false; // 缓冲区满  
        }
    }  
    return true;
} 
