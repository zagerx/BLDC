#include "sensor.h"

float sensor_readanlge(void)
{
    return mt6816_readangle();
}
