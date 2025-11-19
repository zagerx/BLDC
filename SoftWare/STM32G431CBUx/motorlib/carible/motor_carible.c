#include "motor_carible.h"
#include "motor.h"
enum m_carible motor_get_calibstate(struct device *motor)
{
    struct motor_data *m_data = motor->data;
    struct motor_calibration_modules *m_calib = m_data->calib;
    return m_calib->state;
}
void motor_set_calibstate(struct device *motor,enum m_carible state)
{
    struct motor_data *m_data = motor->data;
    struct motor_calibration_modules *m_calib = m_data->calib;
    m_calib->state = state;
}