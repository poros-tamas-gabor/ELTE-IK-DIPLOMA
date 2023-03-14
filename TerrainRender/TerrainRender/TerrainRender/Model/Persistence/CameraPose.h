#ifndef CAMERA_POSE_H
#define CAMERA_POSE_H

struct CameraPose {
	long sec;
	long nsec;

	double yaw;
	double pitch;
	double roll;

	double north;
	double east;
	double down;
};
#endif