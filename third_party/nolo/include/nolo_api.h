#pragma once
#ifndef _NOLO_V2_API_H_
#define _NOLO_V2_API_H_
#define WIN32_LEAN_AND_MEAN    
#include <windows.h>
#include <math.h>

#ifndef NOLO_API
#define NOLO_API extern "C"
#else
#define NOLO_API
#endif
///Nolo_USBHID.dll api
namespace NOLO {
	typedef struct Vector2
	{
		float x;
		float y;
	}Vector2;
	typedef struct Vector3
	{
		float x;
		float y;
		float z;
        //#ifdef __Cplusplus
		Vector3() {
		
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		Vector3(float _x, float _y, float _z) {

			x = _x;
			y = _y;
			z = _z;
		}

		Vector3 operator -(const Vector3 rhs) {
			Vector3 rhs_const_copy(rhs);
			return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
		}
		float operator *(const Vector3 rhs) {

			Vector3 rhs_const_copy(rhs);
			return x*rhs_const_copy.x + y*rhs_const_copy.y + z*rhs_const_copy.z;
		}
		Vector3 operator *(const float rhs) {

			float rhs_const_copy(rhs);
			return Vector3(x*rhs_const_copy, y*rhs_const_copy, z*rhs_const_copy);
		}
		Vector3 operator +(const Vector3 rhs) {

			Vector3 rhs_const_copy(rhs);
			return Vector3(x + rhs_const_copy.x, y + rhs_const_copy.y, z + rhs_const_copy.z);
		}
		Vector3 operator -(const float rhs) {

			float rhs_const_copy(rhs);
			return Vector3(x - rhs_const_copy, y - rhs_const_copy, z - rhs_const_copy);
		}
		Vector3 operator / (const float rhs) {

			float rhs_const_copy(rhs);
			return Vector3(x / rhs_const_copy, y / rhs_const_copy, z / rhs_const_copy);
		}
		Vector3 operator = (const Vector3 & rhs) {

			Vector3 rhs_const_copy(rhs);
			x = rhs_const_copy.x;
			y = rhs_const_copy.y;
			z = rhs_const_copy.z;
			return *this;
		}
	    bool operator == (const Vector3 & rhs) {
	
		    Vector3 rhs_const_copy(rhs);
		    if (x == rhs_const_copy.x && y == rhs_const_copy.y && z == rhs_const_copy.z) return true;
		    else return false;
	    }
		float length() {
			return sqrt(x * x + y * y + z * z);
		}

		void normalize() {
			float len = length();
			x /= len;
			y /= len;
			z /= len;
		}
	    //#endif
	}Vector3;

	typedef struct Quaternion
	{
		float x;
		float y;
		float z;
		float w;
		//#ifdef __Cplusplus
		Quaternion() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 1.0f;
		}
		Quaternion(const float _x, const  float _y, const float _z, const float _w) {
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}
		friend Quaternion operator*(const Quaternion qa, const Quaternion qb) {

			Quaternion qa_const_copy(qa), qb_const_copy(qb), qc;

			qc.w = qa_const_copy.w*qb_const_copy.w - qa_const_copy.x *qb_const_copy.x
				- qa_const_copy.y*qb_const_copy.y - qa_const_copy.z*qb_const_copy.z;

			qc.x = qa_const_copy.x*qb_const_copy.w + qa_const_copy.w *qb_const_copy.x
				+ qa_const_copy.y*qb_const_copy.z - qa_const_copy.z*qb_const_copy.y;

			qc.y = qa_const_copy.y*qb_const_copy.w + qa_const_copy.w *qb_const_copy.y
				+ qa_const_copy.z*qb_const_copy.x - qa_const_copy.x*qb_const_copy.z;

			qc.z = qa_const_copy.z*qb_const_copy.w + qa_const_copy.w *qb_const_copy.z
				+ qa_const_copy.x*qb_const_copy.y - qa_const_copy.y*qb_const_copy.x;
			return qc;
		}
	    //#endif
	}Quaternion;

    typedef enum EBattery
    {
	    ShutDown=0,
	    Low,
	    Middle,
	    High,

    }EBattery;

	typedef enum EDoubleClickKeyType {

		press = 0,
		trigger,
		menu,
		grip,
		cancel
	}EDoubleClickKeyType;

	//Struct of NOLO controller data  【NOLO 手柄数据结构体】
	typedef struct Controller
	{

		Vector3 ControllerPosition;

		Quaternion ControllerRotation;
        /*
		  0x0000_XXX1:press
          0x0000_XX1X:trigger
          0x0000_X1XX:menu
          0x0000_1XXX:system
          0x0001_XXXX:grip
		*/
		UINT Buttons;
        /*
		  0~1 Touchpad  touched:1; Not touched:0，【只要触摸了触摸板状态就为1,没有触摸则为0】
		*/
		int ControllerTouched;
        /*
		   x:-1~1
		   y:-1~1
		*/
		Vector2 ControllerTouchAxis;
        /*
		  0~100：normal,【正常显示电量为0~100】
		  255:Shut down,【长按system按键直到关闭手柄，值为255】
		*/
		EBattery ControllerBattery;
		/* Velocity of the pose in meters/second */
	    Vector3 vecVelocity;

	    /* Acceleration of the pose in meters/second */
	    Vector3 vecAcceleration;

	    /* Angular velocity of the pose in axis-angle 
	     * representation. The direction is the angle of
	     * rotation and the magnitude is the angle around
	     * that axis in radians/second. */
	    Vector3 vecAngularVelocity;

	    /* Angular acceleration of the pose in axis-angle 
	     * representation. The direction is the angle of
	     * rotation and the magnitude is the angle around
	     * that axis in radians/second^2. */
	    Vector3 vecAngularAcceleration;
		 /*
		  0：Be blocked,【被遮挡】
		  1：normal
		*/
		int state;
		
	}Controller;
	//Struct of NOLO headset marker data 【NOLO 头盔定位器数据结构体】
	typedef struct HMD
	{

		Vector3 HMDPosition;
		/*
		  When pressing the pair button on the headset marker,
		  we record the position data of the headset to calibrate the height of the ground.
		  Open vr don't need this data.
		*/
		Vector3 HMDInitPosition;

		Quaternion HMDRotation;
		/* Velocity of the pose in meters/second */
		Vector3 vecVelocity;

		/* Acceleration of the pose in meters/second */
		Vector3 vecAcceleration;

		/* Angular velocity of the pose in axis-angle
		* representation. The direction is the angle of
		* rotation and the magnitude is the angle around
		* that axis in radians/second. */
		Vector3 vecAngularVelocity;

		/* Angular acceleration of the pose in axis-angle
		* representation. The direction is the angle of
		* rotation and the magnitude is the angle around
		* that axis in radians/second^2. */
		Vector3 vecAngularAcceleration;
        /*
		  0：Be blocked
		  1：normal
		*/
		int state;
	}HMD;
	//Struct of NOLO base station data 【NOLO基站数据结构体】
	typedef struct BaseStation
	{
	    Vector3  vecPosition;
        /*
		  0~100:normal
		*/
		EBattery BaseStationPower;
	}BaseStation;
	//Struct of all NOLO devices data 【NOLO所有数据结构体】
	typedef struct NoloData {
        
		Controller left_Controller_Data;

		Controller right_Controller_Data;

		HMD hmdData;

		BaseStation baseStationData;

	}NoloData;
    // Enumerarion of NOLO devices types 【NOLO设备类型枚举】
	typedef enum NoloDeviceType
	{
		HmdDevice = 0,
		LeftControllerDevice,
		RightControllerDevice,
		BaseStationDevice
	}NoloDeviceType;

	typedef enum ExpandMsgType {
	  //180 degree turn-around
	  DoubleClickKeyTurnAround=1,
	  //recenter
	  DoubleClickSystemRecenter
	  
    }ExpandMsgType;
	
	typedef void(__cdecl * noloDataNotifyCallBack)(NoloData _noloData , void * context);
	typedef void(__cdecl *funcCallBack)(void * context);
	typedef void(__cdecl *expandMsgCallBack)(ExpandMsgType expandMsgType , void * context);
	

	/**
	  ******************************************************************************
	  * Function description：Search for NOLO device
	  * Parameter：NULL
	  * Return Value：NULL
	  * Remarks：NULL
	  *【搜索NOLO设备】
	  ******************************************************************************
	*/
	NOLO_API  void _cdecl search_Nolo_Device();
 	/*
      ******************************************************************************
      * Function description：Close the connection with NOLO device and release resources
      * Parameter：NULL
      * Return Value：NULL
      * Remarks：NULL
	  *【关闭与NOLO设备连接，释放资源】
      ******************************************************************************
   */
	NOLO_API  void _cdecl close_Nolo_Device();
    /*
      ******************************************************************************
      * Function description：Get call-back notification when disconnect NOLO device
      * Parameter：Parameter fun ,custom function pointer
      * Return Value：Returns the function registration status:false or true
      * Remarks：NULL
	  *【断开连接NOLO设备将会回调通知】
      ******************************************************************************
    */
	NOLO_API  bool _cdecl registerDisConnectCallBack(funcCallBack fun , void * context);
	/*
      ******************************************************************************
      * Function description：Get call-back notification when connect NOLO device
      * Parameter：Parameter fun ,custom function pointer
      * Return Value：Returns the function registration status:false or true
      * Remarks：NULL
	  *【连接NOLO设备时将会回调通知】
      ******************************************************************************
    */
	NOLO_API  bool _cdecl registerConnectSuccessCallBack(funcCallBack fun , void * context);
		/*
      ******************************************************************************
      * Function description：Interface of Double click the customizable button( The Default is the menu button)  or the system button to notify in real time
      * Parameter：Parameter fun ,custom function pointer
      * Return Value：Returns the function registration status:false or true
      * Remarks：无
	  *【双击自定义按键（默认为双击menu按键）或双击system按键回调通知接口函数】
      ******************************************************************************
    */
	NOLO_API  bool _cdecl registerExpandDataNotifyCallBack(expandMsgCallBack fun , void * context);
	/*
      ******************************************************************************
      * Function description：Get call-back notification whenever getting new data through USB
      * Parameter：Parameter fun ,custom function pointer
      * Return Value：Returns the function registration status:false or true
      * Remarks：NULL
	  *【从usb接收到新数据就回调通知，返回注册函数是否注册成功】
      ******************************************************************************
    */
	NOLO_API  bool _cdecl registerNoloDataNotifyCallBack(noloDataNotifyCallBack fun , void * context);
	/*
      ******************************************************************************
      * Function description：Set vibration functions of NOLO controller
      * Parameter：Parameter devicetype is an enumeration type,Parameter intensity : Vibration intensity range from 0~100, the bigger the stronger
      * Return Value：Return bool type
      * Remarks：Parameter devicetype ：LeftControllerDevice、RightControllerDevice
	  *【设置NOLO手柄震动函数，参数类型为设备类型和震动强度，设备类型:左手和右手，震动强度范围0~100,值越大震动越激烈】
      ******************************************************************************
    */
	NOLO_API  bool _cdecl set_Nolo_TriggerHapticPulse(NoloDeviceType deviceType, int intensity);
	/*
      ******************************************************************************
      * Function description：Get all data interface function of NOLO
      * Parameter：NULL
      * Return Value：Returns the NoloData structure data
      * Remarks：NULL
	  *【获取NOLOs所有数据接口函数】
      ******************************************************************************
    */
	NOLO_API  NoloData _cdecl get_Nolo_NoloData();
	/*
      ******************************************************************************
      * Function description：Get all data interface function of NOLO left controller
      * Parameter：NULL
      * Return Value：Returns the Controller structure data
      * Remarks：NULL
	  *【获取NOLO左手柄数据接口函数】
      ******************************************************************************
    */
	NOLO_API  Controller _cdecl get_Nolo_LeftControllerData();
	/*
      ******************************************************************************
      * Function description：Get all data interface function of NOLO right controller
      * Parameter：NULL
      * Return Value：Returns the Controller structure data
      * Remarks：NULL
	  *【获取NOLO右手柄数据接口函数】
      ******************************************************************************
    */
	NOLO_API  Controller _cdecl get_Nolo_RightControllerData();
	/*
      ******************************************************************************
      * Function description：Get all data interface function of NOLO headset marker
      * Parameter：NULL
      * Return Value：Returns the HMD structure data
      * Remarks：NULL
	  *【获取NOLO头部数据接口函数】
      ******************************************************************************
    */
	NOLO_API HMD _cdecl get_Nolo_HMDData();
	/*
      ******************************************************************************
      * Function description：Get all data interface function of NOLO base station
      * Parameter：NULL 
      * Return Value：Returns the BaseStation structure data
      * Remarks：NULL
	  *【获取NOLO基站数据接口函数】
      ******************************************************************************
    */
	NOLO_API BaseStation _cdecl get_Nolo_BaseStationData();
	/*
      ******************************************************************************
      * Function description：Get status data of NOLO battery power based on the type of device
      * Parameter：Parameter devicetype is an enumeration type
      * Return Value：Returns the EBattery enumeration type:shut down 、low 、middle 、high
      * Remarks：NULL
	  *【根据设备类型获取NOLO设备电量数据】
      ******************************************************************************
    */
	NOLO_API EBattery _cdecl get_Nolo_Battery(NoloDeviceType deviceType);
	/*
	******************************************************************************
	* Function description： Customize the 180 degree turn-around hot key by the button types. The default is double click the menu button
	* Parameter：Parameter keyType is an enumeration type
	* Return Value：NULL
	* Remarks：NULL
	*【根据按键类型自定义一键转身热键（旋转180度）,不进行自定义按键时默认为双击menu按键】
	******************************************************************************
	*/
	NOLO_API void _cdecl set_Nolo_TurnAroundKey(EDoubleClickKeyType keyType);
}
#endif // _NOLO_API_Version2_H_