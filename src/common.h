#ifndef OPENVR_SURVIVOR_SRC_COMMON_H_
#define OPENVR_SURVIVOR_SRC_COMMON_H_
#include <iostream>
#include <windows.h>
//In order to prevent ERROR and GLOG / windows.h in the header file ERROR to redefine the problem. 
//This macro must be defined in the logging.h contains before
#define GLOG_NO_ABBREVIATED_SEVERITIES 
#include <glog/logging.h>
#include <openvr_driver.h>
#include <thread>
#include <chrono>
#include "function_configure.h"

//namespace
using namespace std;
using namespace vr;
using namespace google;

//import lib
#pragma comment(lib,"libglog.lib")

inline HmdQuaternion_t HmdQuaternion_Init( double w, double x, double y, double z )
{
	HmdQuaternion_t quat;
	quat.w = w;
	quat.x = x;
	quat.y = y;
	quat.z = z;
	return quat;
}

inline void HmdMatrix_SetIdentity( HmdMatrix34_t *pMatrix )
{
	pMatrix->m[0][0] = 1.f;
	pMatrix->m[0][1] = 0.f;
	pMatrix->m[0][2] = 0.f;
	pMatrix->m[0][3] = 0.f;
	pMatrix->m[1][0] = 0.f;
	pMatrix->m[1][1] = 1.f;
	pMatrix->m[1][2] = 0.f;
	pMatrix->m[1][3] = 0.f;
	pMatrix->m[2][0] = 0.f;
	pMatrix->m[2][1] = 0.f;
	pMatrix->m[2][2] = 1.f;
	pMatrix->m[2][3] = 0.f;
}

typedef enum HAND_CONTROLLER{
    LEFT_HAND_CONTROLLER = 0,
	RIGHT_HAND_CONTROLLER,
	HAND_CONTROLLER_COUNT
}EHandController;

//unit:meter
#define HAND_CONTROLLER_RELATIVE_HMD_POSITION_X			0.2
#define HAND_CONTROLLER_RELATIVE_HMD_POSITION_Y			0.2
#define HAND_CONTROLLER_RELATIVE_HMD_POSITION_Z			0.5
#endif
