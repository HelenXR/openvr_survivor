/** @file 
	@brief project common head file

	@date 2017.05.25
	@author helenxr(helenhololens@gmail.com)
	
	if you want,you can use this in everywhere.
	if you want to contribute to this project,join us.
*/

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
#include "glm.hpp"
#include "glm_adapter.h"
#include "simple_math.h"

//namespace
using namespace std;
using namespace vr;
using namespace google;

//import lib
#pragma comment(lib,"libglog.lib")
#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
#pragma comment(lib,"xdevice.lib")
#include <xdevice.h>
using namespace ximmerse;
//glm
#include <gtx/euler_angles.hpp>
#include <gtx/quaternion.hpp>
#endif
#ifdef USE_NOLO_SIX_DOF_TRACKING_MODULE
#pragma comment(lib,"NoLo_USBHID.lib")
#include "nolo_api.h"
using namespace NOLO;
#endif

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

//keyboard for controller button struct
typedef struct KeyBoardForControllerButton_{
	uint16_t ButtonState;
	VRControllerAxis_t rAxis[k_unControllerStateAxisCount];
}KeyBoardForControllerButton;

#define CONTROLLER_BUTTON_MENU				(1<<0)
#define CONTROLLER_BUTTON_PAD_LEFT			(1<<1)
#define CONTROLLER_BUTTON_PAD_UP			(1<<2)
#define CONTROLLER_BUTTON_PAD_RIGHT			(1<<3)
#define CONTROLLER_BUTTON_PAD_DOWN			(1<<4)
#define CONTROLLER_BUTTON_SYSTEM			(1<<5)
#define CONTROLLER_BUTTON_TRIGGER			(1<<6)
#define CONTROLLER_BUTTON_GRIP				(1<<7)
#define CONTROLLER_BUTTON_TRACKEPAD_PRESS	(1<<8)

#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
typedef enum{
	XIMMERSE_HAWK 								= 0,
	XIMMERSE_COBRA_0 							= 1,
	XIMMERSE_COBRA_1							= 2,
	XIMMERSE_VRDEVICE							= 3,
	XIMMERSE_DEVICE_MAX							= 4
}EXimmerseHandle;

#define DOUBLE_CLICK_INTERVAL			150 //150ms
#endif

typedef enum {
    NONE_SIX_DOF_TRACKING_MODULE 				= 0,
	NOLO_SIX_DOF_TRACKING_MODULE				= 1,
	XIMMERSE_SIX_DOF_TRACKING_MODULE			= 2
}ESixDofTrackingModule;
	
#define NONE_SIX_DOF_CONTROLLER_POSE_REPORT_INTERVAL				1000	
#define NOLO_SIX_DOF_CONTROLLER_POSE_REPORT_INTERVAL				8
#define XIMMERSE_SIX_DOF_CONTROLLER_POSE_REPORT_INTERVAL			8

typedef enum POSTMESSAGETYPE{
    POST_MESSAGE_START = WM_USER + 1,
	POST_MESSAGE_RECENTER,
	POST_MESSAGE_TURN_AROUND,
	POST_MESSAGE_END
}EPostMessageType;

/** 
	keys for use with the settings API
*/
static const char * const k_pch_Sample_Section = "driver_helenxr";
static const char * const k_pch_Sample_SerialNumber_String = "serialNumber";
static const char * const k_pch_Sample_ModelNumber_String = "modelNumber";
static const char * const k_pch_Sample_WindowX_Int32 = "windowX";
static const char * const k_pch_Sample_WindowY_Int32 = "windowY";
static const char * const k_pch_Sample_WindowWidth_Int32 = "windowWidth";
static const char * const k_pch_Sample_WindowHeight_Int32 = "windowHeight";
static const char * const k_pch_Sample_RenderWidth_Int32 = "renderWidth";
static const char * const k_pch_Sample_RenderHeight_Int32 = "renderHeight";
static const char * const k_pch_Sample_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
static const char * const k_pch_Sample_DisplayFrequency_Float = "displayFrequency";
static const char * const k_pch_Sample_HmdXPositionOffset_Float = "hmd_x_position_offset";
static const char * const k_pch_Sample_HmdYPositionOffset_Float = "hmd_y_position_offset";
static const char * const k_pch_Sample_HmdZPositionOffset_Float = "hmd_z_position_offset";
#ifdef USE_NOLO_SIX_DOF_TRACKING_MODULE
#define NOLO_BUTTON_DPAD_PRESS				(0x01 << 0)
#define NOLO_BUTTON_TRIGGER_PRESS			(0x01 << 1)
#define NOLO_BUTTON_MENU					(0x01 << 2)
#define NOLO_BUTTON_SYSTEM					(0x01 << 3)
#define NOLO_BUTTON_GRIP					(0x01 << 4)
#endif

#endif
