#pragma once 

// xdevice.h 
//========= © 2016 XIMMERSE,LTD. ALL RIGHTS RESERVED. ============// 
// Dynamically generated file. Do not modify this file directly. 

#ifndef __XDEVICE_API__ 
#define __XDEVICE_API__ 

namespace ximmerse { 


#ifndef __XIM_XDEVICE_LOG_H__
#define __XIM_XDEVICE_LOG_H__


	enum LOGLevel {
		LOG_VERBOSE = 0,
		LOG_DEBUG = 1,
		LOG_INFO = 2,
		LOG_WARN = 3,
		LOG_ERROR = 4,
		LOG_OFF = 5,
	};

	enum XHawkFeature {
		FEATURE_BLOB_AUTO = 0x0001,
	};

#endif
#ifndef __XIM_FIELD_ID_H__
#define __XIM_FIELD_ID_H__

// TODO : kField_[Ctx]NameType

enum FieldID/*:int*/{
	// Bool
	kField_BoolOffset=0,
	kField_CtxCanProcessInputEventBool=kField_BoolOffset,
	kField_CtxBoolMax,
	kField_AutoProcessInputEventBool=kField_BoolOffset,
	kField_CanProcessInputEventBool,
	kField_CanProcessAxisEventBool,
	kField_CanProcessKeyEventBool,
	kField_CanProcessPositionEventBool,
	kField_CanProcessAccelerometerEventBool,
	kField_CanProcessRotationEventBool,
	kField_CanProcessGyroscopeEventBool,
	kField_CanCheckAxesRangeBool,
	kField_CanMapAxesToButtonsBool,
	kField_IsAbsRotationBool,
	kField_IsDeviceSleepBool,
	kField_BoolMax,
	// Int
	kField_IntOffset=0,
	kField_CtxSdkVersionInt=kField_IntOffset,
	kField_CtxDeviceVersionInt,
	kField_CtxPlatformVersionInt,
	kField_CtxLogLevelInt,
	kField_VIDInt,
	kField_PIDInt,
	kField_CustomerIDInt,
	kField_CtxIntMax,
	kField_NumAxesInt=kField_IntOffset,
	kField_NumButtonsInt,
	kField_ErrorCodeInt,
	kField_ConnectionStateInt,
	kField_BatteryLevelInt,
	kField_TrackingResultInt,
	kField_TrackingOriginInt,
	kField_FwVersionInt,
	kField_BlobIDInt,
	KField_FPSInt,
	kField_IntMax,
	// Float
	kField_FloatOffset=0,
	kField_PositionScaleFloat=kField_FloatOffset,
	kField_TrackerHeightFloat,
	kField_TrackerDepthFloat,
	kField_TrackerPitchFloat,
	kField_TriggerAsButtonThresholdFloat=kField_FloatOffset,
	kField_AxisAsButtonThresholdFloat,
	kField_AxisDeadzoneThresholdFloat,
	kField_AccelScaleFloat,
	kField_GyroScaleFloat,
	kField_FloatMax,
	// Object
	kField_ObjectOffset=0,
	kField_TRSObject=kField_ObjectOffset,
	kField_DeviceInfoObject,
	kField_AddressObject,
	kField_DisplayNameObject,
	kField_ModelNameObject,
	//kField_ModelNumberObject,
	kField_SerialNumberObject,
	kField_FirmwareRevisionObject,
	kField_HardwareRevisionObject,
	kField_SoftwareRevisionObject,
	kField_ManufacturerNameObject,
	kField_ObjectMax,
};

#endif

#ifndef __XIM_MESSAGE_ID_H__
#define __XIM_MESSAGE_ID_H__

enum MessageID{
	kMessage_DispatchInputEvent,
	kMessage_TriggerVibration,
	kMessage_RecenterSensor,
	kMessage_SleepMode,
	kMessage_UpdateDisplayStrings,
	kMessage_DeviceOperation,
	kMessage_ChangeBlobColorTemp,
    kMessage_ChangeBlobColorDefault,
    kMessage_SetDeviceName,
    kMessage_SetModelName
};

#endif

#ifndef __XIM_EVENT_ID_H__
#define __XIM_EVENT_ID_H__

enum EventID/*:int */{
	kEvent_UpdateInputState,
	kEvent_ProcessInputEvent,
	kEvent_ProcessAxisEvent,
	kEvent_ProcessKeyEvent,
	kEvent_ProcessPositionEvent,
	kEvent_ProcessRotationEvent,
	kEvent_SendMessage,
	kEvent_Recenter,
};

#endif

#ifndef __XIM_INPUT_DEVICE_H__
#define __XIM_INPUT_DEVICE_H__

enum DeviceType {
	kDeviceType_Unknown,
	kDeviceType_ControllerDevice,
	kDeviceType_TrackerDevice,
};

enum ConnectionState {
	kConnectionState_Disconnected,
	kConnectionState_Scanning,
	kConnectionState_Connecting,
	kConnectionState_Connected,
	kConnectionState_Error,
};

#endif

#ifndef __XIM_CONTROLLER_STATE_H__
#define __XIM_CONTROLLER_STATE_H__

// Reference : https://msdn.microsoft.com/en-us/library/windows/apps/microsoft.directx_sdk.reference.xinput_gamepad
enum ControllerAxis {
	CONTROLLER_AXIS_LEFT_TRIGGER,
	CONTROLLER_AXIS_RIGHT_TRIGGER,
	CONTROLLER_AXIS_LEFT_THUMB_X,
	CONTROLLER_AXIS_LEFT_THUMB_Y,
	CONTROLLER_AXIS_RIGHT_THUMB_X,
	CONTROLLER_AXIS_RIGHT_THUMB_Y,
	CONTROLLER_AXIS_MAX,
};

// Reference : https://msdn.microsoft.com/en-us/library/windows/apps/microsoft.directx_sdk.reference.xinput_gamepad
enum ControllerButton {
	CONTROLLER_BUTTON_DPAD_UP        = 0x0001,
	CONTROLLER_BUTTON_DPAD_DOWN      = 0x0002,
	CONTROLLER_BUTTON_DPAD_LEFT      = 0x0004,
	CONTROLLER_BUTTON_DPAD_RIGHT     = 0x0008,
	CONTROLLER_BUTTON_START          = 0x0010,
	CONTROLLER_BUTTON_BACK           = 0x0020,
	CONTROLLER_BUTTON_LEFT_THUMB     = 0x0040,
	CONTROLLER_BUTTON_RIGHT_THUMB    = 0x0080,
	CONTROLLER_BUTTON_LEFT_SHOULDER  = 0x0100,
	CONTROLLER_BUTTON_RIGHT_SHOULDER = 0x0200,
	CONTROLLER_BUTTON_GUIDE          = 0x0400,
	CONTROLLER_BUTTON_A              = 0x1000,
	CONTROLLER_BUTTON_B              = 0x2000,
	CONTROLLER_BUTTON_X              = 0x4000,
	CONTROLLER_BUTTON_Y              = 0x8000,
	// Emulation
	CONTROLLER_BUTTON_LEFT_THUMB_MOVE   = 0x10000,
	CONTROLLER_BUTTON_RIGHT_THUMB_MOVE  = 0x20000,
	CONTROLLER_BUTTON_LEFT_TRIGGER      = 0x40000,
	CONTROLLER_BUTTON_RIGHT_TRIGGER     = 0x80000,
	CONTROLLER_BUTTON_LEFT_THUMB_UP     = 0x100000,
	CONTROLLER_BUTTON_LEFT_THUMB_DOWN   = 0x200000,
	CONTROLLER_BUTTON_LEFT_THUMB_LEFT   = 0x400000,
	CONTROLLER_BUTTON_LEFT_THUMB_RIGHT  = 0x800000,
	CONTROLLER_BUTTON_RIGHT_THUMB_UP    = 0x1000000,
	CONTROLLER_BUTTON_RIGHT_THUMB_DOWN  = 0x2000000,
	CONTROLLER_BUTTON_RIGHT_THUMB_LEFT  = 0x4000000,
	CONTROLLER_BUTTON_RIGHT_THUMB_RIGHT = 0x8000000,
	//
	CONTROLLER_BUTTON_NONE           = 0x0,
	CONTROLLER_BUTTON_ANY            = ~CONTROLLER_BUTTON_NONE,
};

typedef struct tagControllerState{
	// Common
	int handle;
	int timestamp;
	int frameCount;
	int state_mask;
	// Gamepad
	float axes[6];
	unsigned int buttons;
	// Motion
	float position[3];
	float rotation[4];
	float accelerometer[3];
	float gyroscope[3];
}ControllerState;

#endif

#ifndef __XIM_TRACKER_STATE_H__
#define __XIM_TRACKER_STATE_H__

enum TrackingResult{
	kTrackingResult_NotTracked      =    0,
	kTrackingResult_RotationTracked = 1<<0,
	kTrackingResult_PositionTracked = 1<<1,
	kTrackingResult_PoseTracked     = (kTrackingResult_RotationTracked|kTrackingResult_PositionTracked),
};

enum TrackingOrigin{
	kTrackingOrigin_Unknown    =-1,
	kTrackingOrigin_EyeLevel   = 0,
	kTrackingOrigin_FloorLevel = 1,
};

typedef struct tagTrackerState{
	int handle;
	int timestamp;
	int frameCount;
	int capacity;
	int count;
	int* id;
	float* data;
}TrackerState;

#endif

#ifndef __XIM_DELEGATES_H__
#define __XIM_DELEGATES_H__


typedef void(*void_delegate)(int which);
typedef void(*axis_delegate)(int which, int axis, float value);
typedef void(*key_delegate)(int which, int code, int action);
typedef void(*vector3f_delegate)(int which, int node, float x, float y, float z);
typedef void(*vector4f_delegate)(int which, int node, float x, float y, float z, float w);

typedef int(*controller_state_delegate)(int which, ControllerState* state);
typedef int(*send_message_delegate)(int which, int Msg, int wParam, int lParam);

typedef int(*log_delegate)(int level,const char* tag,char* msg);
typedef void(*dfu_delegate)(int which, float level, int state);
#endif
} 

#ifndef __XIM_XDEVICE_H__
#define __XIM_XDEVICE_H__


#ifndef XIM_API
#if defined(_WIN32)||defined(_WIN64)
	#define XIM_API __declspec(dllimport)
#else
	#define XIM_API
#endif
#endif

// Device ID

#define ID_CONTEXT 0xFF

#define XIM_DK3      		0x3000
#define XIM_DK4		 		0x4000
#define XIM_DK4_DIS01		0x4001
#define XIM_RD06   			0x4002 
#define XIM_CV1     		0x4010
#define XIM_CV1_HTC			0x4011
#define XIM_CV1_SAM			0x4012

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif
	// Common
	/** Initialize the X-Device SDK library.*/
	XIM_API int XDeviceInit(void);
	/** Finalize the X-Device SDK library.*/
	XIM_API int XDeviceExit(void);

	// Device Management

	/** Get the current device context of X-Device SDK.If autoCreate is true,it will ensure the context is created.*/
	XIM_API void* XDeviceGetContext(bool autoCreate);
	/** Get the pointer of input device identified by handle.*/
	XIM_API void* XDeviceGetInputDevice(int which);
	/** Get the handle of input device identified by name.*/
	XIM_API int XDeviceGetInputDeviceHandle(char* name);
	/** Get the name of input device identified by handle.*/
	XIM_API char* XDeviceGetInputDeviceName(int which);

	/** Get count of input devices in X-Device SDK.*/
	XIM_API int XDeviceGetInputDeviceCount();
	/** Get handles of all input devices identified by type.*/
	XIM_API int XDeviceGetInputDevices(int type,int* whichBuffer,int whichBufferSize);

	/** Register an Input device with the requested name.*/
	XIM_API int XDeviceAddInputDevice(char* name, void* device);
	/** Unregister an Input device with the requested handle.If dispose is true,it will finalize input device and return NULL.*/
	XIM_API void* XDeviceRemoveInputDeviceAt(int which,bool dispose);
	/** Register an external controller device to X-Device SDK.You can implement converter and sender in external of X-Device SDK.*/
	XIM_API int XDeviceAddExternalControllerDevice(char* name, ximmerse::controller_state_delegate converter, ximmerse::send_message_delegate sender);

	// I/O
	
	/** Get the input state of input device identified by handle.*/
	XIM_API int XDeviceGetInputState(int which, void* state);
	/** Send a message to the input device identified by handle.*/
	XIM_API int XDeviceSendMessage(int which, int Msg, int wParam, int lParam);
	XIM_API int XDeviceSendRecenterMessage(int which, float wParam, int lParam) ;
	
	/** Reset the input state of input device identified by handle.*/
	XIM_API int XDeviceResetInputState(int which);
	/** Update the input device identified by handle.*/
	XIM_API int XDeviceUpdateInputState(int which);
	/** Update the input buffer of input device identified by handle.*/
	XIM_API int XDeviceUpdateInputBuffer(int which, unsigned char* buffer, int offset, int count);
	
	/** Serialize a message from the input device identified by handle.*/
	XIM_API int XDeviceSerializeMessage(unsigned char* buffer, int offset, int count,
		                                int which, int Msg, int wParam, int lParam);
	/** Send a buffer to the input device identified by handle.*/
	XIM_API int XDeviceSendBuffer(int which, unsigned char* buffer, int offset, int count);

	// Event

	/** Set an event listener on the event identified by eventID of the input device identified by handle.*/
	XIM_API int XDeviceSetEventListener(int which, int eventID, void* listener);
	/** Dispatch the event identified by eventID of the input device identified by handle.*/
	XIM_API int XDeviceDispatchEventListener(int which, int eventID);

	// Get/Set

	/** Get a bool value of input device identified by handle.*/
	XIM_API bool XDeviceGetBool(int which, int fieldID, bool defaultValue);
	/** Set a bool value of input device identified by handle.*/
	XIM_API void XDeviceSetBool(int which, int fieldID, bool value);
	/** Get an int value of input device identified by handle.*/
	XIM_API int XDeviceGetInt(int which, int fieldID, int defaultValue);
	/** Set an int value of input device identified by handle.*/
	XIM_API void XDeviceSetInt(int which, int fieldID, int value);
	/** Get a float value of input device identified by handle.*/
	XIM_API float XDeviceGetFloat(int which, int fieldID, float defaultValue);
	/** Set a float value of input device identified by handle.*/
	XIM_API void XDeviceSetFloat(int which, int fieldID, float value);
	/** Get a object value of input device identified by handle.*/
	XIM_API void XDeviceGetObject(int which, int fieldID, unsigned char* buffer, int offset, int count);
	/** Set a object value of input device identified by handle.*/
	XIM_API void XDeviceSetObject(int which, int fieldID, unsigned char* buffer, int offset, int count);
	/** Get a string value of input device identified by handle.*/
	XIM_API char* XDeviceGetString(int which, int fieldID, char* defaultValue);
	/** Set a string value of input device identified by handle.*/
	XIM_API void XDeviceSetString(int which, int fieldID, char* value);

	//Ext

	/** Get a field id identified by name.*/
	XIM_API int XDeviceGetFieldID(char* name);

	/** Get a event id identified by name.*/
	XIM_API int XDeviceGetEventID(char* name);

	/** Get a message id identified by name.*/
	XIM_API int XDeviceGetMessageID(char* name);

	/** Get the node position of input device identified by handle.*/
	XIM_API int XDeviceGetNodePosition(int which, int history, int node, float* position);

	/** Update the node pose(position and rotation) of input device identified by handle.*/
	XIM_API int XDeviceUpdateNodePose(int which, int node, float* position, float* rotation);

	/** */
	XIM_API int XDeviceGetTrackerPose(int which,float* height,float* depth,float* pitch);

	/** */
	XIM_API int XDeviceSetTrackerPose(int which,float height,float depth,float pitch);

	/** */
	XIM_API int XDeviceGetTickCount();

	/** */
	XIM_API unsigned int XDeviceGetTickCount1us();

	/** */
	XIM_API int XDeviceSetLogger(ximmerse::log_delegate logger);

	XIM_API int XDeviceSetDufMessage(ximmerse::dfu_delegate message);

	XIM_API bool GetDecoderEvent(int type, void * paramStruct, void * eventStruct);

	XIM_API bool   SetHeadRotation(float quaternion[],float pitch=0,float yaw=0,float roll=0);

	XIM_API float XDeviceGetHmdYaw0(float* rotation, float confidence);
	XIM_API float XDeviceGetHmdYaw(float* rotation);

	XIM_API bool XDeviceProcessDFU(int device, int curVersion, const char* hexFile,  void * delegate);

	XIM_API void XDeviceSetBleScanCallback(int (*callback)(void*));
	XIM_API int XDeviceGetDecoderFeature();
	XIM_API bool XDeviceSetDecoderFeature(int feature);

	XIM_API int XDeviceGetHmdPredictionPosition(float* position);
#define PredictionEventID 0x01
#define RecenterEventID   0x02
	struct PredictionEvent
	{
		int flag;
		float position[3];              // position (x, y, z)  mm
		float linearVelocity[3];        // linear velocity
		float predictPosition[3];       // predicted position
		float   linearPredictVelocity[3]; // predicted linear velocity
	};
	struct RecenterEvent
	{
		float rotation[4];              // rotation in quaternion (qx, qy, qz, qw)
		float yaw;                      // lastest yaw
		float predictYaw;               // predicted yaw based on timestamp
		int   predictEnable;            // whether predicted is enable/on, 1 enable, -1 disable
		unsigned long long timestamp;   // timestamp of the lastest yaw
	};
	struct RecenterParam
	{
		float rotation[4];              // rotation in quaternion (qx, qy, qz, qw)
		unsigned long long timestamp;   // timestamp
	};

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif

#ifndef __XIM_PLAYER_PREFS_H__
#define __XIM_PLAYER_PREFS_H__

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

XIM_API void PlayerPrefs_Init(char* key);

XIM_API char* PlayerPrefs_GetString(char* key,char* subkey,char* defaultValue);
XIM_API void PlayerPrefs_SetString(char* key,char* subkey,char* value);

XIM_API int PlayerPrefs_GetInt(char* key,char* subkey,int defaultValue);
XIM_API void PlayerPrefs_SetInt(char* key,char* subkey,int value);

XIM_API float PlayerPrefs_GetFloat(char* key,char* subkey,float defaultValue);
XIM_API void PlayerPrefs_SetFloat(char* key,char* subkey,float value);

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif

#ifndef __XIM_JUGGLER_H__
#define __XIM_JUGGLER_H__

#if !defined(PLATFORM_WIN)
#if defined(_WIN32)
#define PLATFORM_WIN 1
#endif
#endif

#if PLATFORM_WIN
#include <windows.h>
#endif
#include <vector>
#include <mutex>

class IAnimatable{
public:
	virtual void AdvanceTime(int time)=0;
	virtual void Abort()=0;
};

class Juggler;
class XIM_API DelayedCall:public IAnimatable{

public:
	Juggler* juggler;
	int time;
	void* function;
	int arg0;
	int arg1;

public:
	DelayedCall(Juggler* juggler,int time,void* function,int arg0,int arg1);
	virtual ~DelayedCall();

public:
	virtual void AdvanceTime(int time)override;
	virtual void Run();
	virtual void Abort()override;
};

template<typename T>
class TypedDelayedCall:public DelayedCall{

public:
	T* thiz;
	void (T::*thiz_func)(int,int);
	TypedDelayedCall(Juggler* juggler,int time,T* thiz,void (T::*thiz_func)(int,int),int arg0,int arg1)
		: DelayedCall(juggler,time,NULL,arg0,arg1)
		,thiz(thiz)
		,thiz_func(thiz_func){
	}
	virtual ~TypedDelayedCall(){}
	virtual void Run()override{ (thiz->*thiz_func)(arg0,arg1); }
};

class XIM_API Juggler:public IAnimatable{
public:
	static Juggler* Main();
	static void Exit();
protected:
	static Juggler* s_Main;

#if PLATFORM_WIN
protected:
	static DWORD WINAPI ThreadFunc(LPVOID lpParam);
	HANDLE m_Thread;
#endif

public:
	int elapsedTime;
	int timeStep;
	IAnimatable* current;

protected:
	std::vector<IAnimatable*> m_IAnimatables;
	std::mutex m_Mutex;

public:
	Juggler(int timeStep=50);
	virtual ~Juggler();

public:
	virtual int TickCount();
	virtual void AdvanceTime(int time)override;
	virtual void Abort()override;

	virtual void Add(IAnimatable* animatable);
	virtual void Remove(IAnimatable* animatable);

	template<typename T>
	DelayedCall* DelayCall(T* thiz,void (T::*function)(int,int),int delay,int arg0=0,int arg1=0);
};

// Inline functions

#if PLATFORM_WIN
inline int Juggler::TickCount(){
	return GetTickCount();
}
#endif

template<typename T>
inline DelayedCall* Juggler::DelayCall(T* thiz,void (T::*function)(int,int),int delay,int arg0,int arg1){
	//
	DelayedCall* ret=new TypedDelayedCall<T>(this,delay,thiz,function,arg0,arg1);
	Add(ret);
	//
	return ret;
}

#endif

#endif 
