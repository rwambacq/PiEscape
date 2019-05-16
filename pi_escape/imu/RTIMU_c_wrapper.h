#ifdef __cplusplus
extern "C" {
#endif
#ifndef PIESCAPE2_RTIMU_C_WRAPPER_H
#define PIESCAPE2_RTIMU_C_WRAPPER_H

void sensor_imu_init();
void check_new_imudata();
float get_imu_x();
float get_imu_y();
float get_imu_z();

#endif //PIESCAPE2_RTIMU_C_WRAPPER_H
#ifdef __cplusplus
}
#endif
