#include "RTIMU_c_wrapper.h"

RTIMU_DATA imuData;
RTIMU *imu;

void sensor_imu_init() {
	RTIMUSettings *settings = new RTIMUSettings("RTIMULib");
	imu = RTIMU::createIMU(settings);
	imu->IMUInit();
}

void check_new_imudata() {
	if (imu->IMUread()) {
		imuData = imu->getIMUData();
	}
}

float get_imu_x() {
	check_new_imudata();
	float value = (float)imuData.fusionPose.x();
	return value;
}

float get_imu_y() {
	check_new_imudata();
	float value = (float)imuData.fusionPose.y();
	return value;
}

float get_imu_z() {
	check_new_imudata();
	float value = (float)imuData.fusionPose.z();
	return value;
}


