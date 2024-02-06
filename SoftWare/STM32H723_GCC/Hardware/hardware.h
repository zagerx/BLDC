#ifndef __HARDWARE__H
#define __HARDWARE__H



#ifdef ANGLE_SENSOR
    void* angle_read(void);
#endif

void* ina226_read_data(void);
#endif
