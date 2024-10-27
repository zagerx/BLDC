#include "voft.h"
#include "usart.h"
#include "string.h"
void votf_sendf(float *pdata,uint16_t datalen)
{
#ifdef VOTF_SOFTWARE  
  uint8_t buf[64];
  uint8_t bytelen;
  bytelen = datalen*4;
  memcpy(&buf[0],(uint8_t*)pdata,datalen*4);
  buf[bytelen] = 0x00;
  buf[bytelen+1] = 0x00;
  buf[bytelen+2] = 0x80;
  buf[bytelen+3] = 0x7f;
  _bsp_protransmit(buf,4+bytelen);
#endif  
}


