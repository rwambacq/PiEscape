#ifdef __cplusplus
#ifndef PIESCAPE2_RTIMU_C_WRAPPER_H
#define PIESCAPE2_RTIMU_C_WRAPPER_H

#include "RTIMULib.h"

void sensor_imu_init();
void check_new_imudata();
float get_imu_x();
float get_imu_y();
float get_imu_z();

#endif //PIESCAPE2_RTIMU_C_WRAPPER_H
#endif
