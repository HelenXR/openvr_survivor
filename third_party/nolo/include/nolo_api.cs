using System;
using System.Runtime.InteropServices;


namespace NOLO
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct NVector3
    {
        public float x;
        public float y;
        public float z;

    }
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct NVector2
    {
        public float x;
        public float y;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct NQuaternion
    {
        public float x;
        public float y;
        public float z;
        public float w;
    }

    public enum EBattery{

        ShutDown = 0,
        Low,
        Middle,
        High
    }
    public enum EDoubleClickKeyType{

        press = 0,
		trigger,
		menu,
		grip,
		cancel
    }
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
   	//Struct of NOLO controller data  【NOLO 手柄数据结构体】
    public struct Controller
    {

        public NVector3 ControllerPosition;

        public NQuaternion ControllerRotation;
        /*
		  0x0000_XXX1:press
          0x0000_XX1X:trigger
          0x0000_X1XX:menu
          0x0000_1XXX:system
          0x0001_XXXX:grip
		*/
        public uint Buttons;
        /**
		  0~1 Touchpad  touched:1; Not touched:0，【只要触摸了触摸板状态就为1,没有触摸则为0】
		*/
		public int ControllerTouched;
        /*
		   x:-1~1
		   y:-1~1
		*/
        public NVector2 ControllerTouchAxis;
        /*
		  0~100：normal
		  255:Shut down
		*/
        public EBattery ControllerBattery;

		/* Velocity of the pose in meters/second */
	    NVector3 vecVelocity;

	    /* Acceleration of the pose in meters/second */
	    NVector3 vecAcceleration;

	    /* Angular velocity of the pose in axis-angle 
	     * representation. The direction is the angle of
	     * rotation and the magnitude is the angle around
	     * that axis in radians/second. */
	    NVector3 vecAngularVelocity;

	    /* Angular acceleration of the pose in axis-angle 
	     * representation. The direction is the angle of
	     * rotation and the magnitude is the angle around
	     * that axis in radians/second^2. */
	    NVector3 vecAngularAcceleration;

        /*
		  0：Be blocked
		  1：normal
		*/
        public int state;
    };
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    //Struct of NOLO headset marker data 【NOLO 头盔定位器数据结构体】
    public struct HMD
    {

        public NVector3 HMDPosition;
        /*
		  When pressing the pair button on the headset marker,
  		  we record the position data of the headset to calibrate the height of the ground.
		  Open vr don't need this data.
		*/
		public NVector3 HMDInitPosition;

        public NQuaternion HMDRotation;
		/* Velocity of the pose in meters/second */
	    NVector3 vecVelocity;

	    /* Acceleration of the pose in meters/second */
	    NVector3 vecAcceleration;

	    /* Angular velocity of the pose in axis-angle 
	     * representation. The direction is the angle of
	     * rotation and the magnitude is the angle around
	     * that axis in radians/second. */
	    NVector3 vecAngularVelocity;

	    /* Angular acceleration of the pose in axis-angle 
	     * representation. The direction is the angle of
	     * rotation and the magnitude is the angle around
	     * that axis in radians/second^2. */
	    NVector3 vecAngularAcceleration;
        /*
		  0：Be blocked
		  1：normal
		*/
        public int state;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    //Struct of NOLO base station data 【NOLO基站数据结构体】
    public struct BaseStation
    {
        NVector3  vecPosition;
        /*
		  0~100:normal
		*/
        public int BaseStationPower;
    }
    // Enumerarion of NOLO devices types 【NOLO设备类型枚举】
    public enum NoloDeviceType
    {
        HmdDevice = 0,
        LeftControllerDevice = 1,
        RightControllerDevice = 2,
        BaseStationDevice = 3,
    };

    ///
    public enum ExpandMsgType
    {
	    //180 degree turn-around
        DoubleClickKeyTurnAround = 1,
		//recenter
	    DoubleClickSystemRecenter
    };
    
    public struct NoloData{

        public Controller left_Controller_Data;
        public Controller right_Controller_Data;
        public HMD hmdData;
        public BaseStation baseStationData;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcCallback(IntPtr context);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate void expandMsgFunc(ExpandMsgType msg ,IntPtr context);
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate void noloDataNotifyFunc(NoloData noloData ,IntPtr context);
    public class NOLOClient_V2_API
    {
	    /**
          ******************************************************************************
          * Function description：Search for NOLO device
          * Parameter：NULL
          * Return Value：NULL
          * Remarks：NULL
	      *【搜索NOLO设备】
          ******************************************************************************
       */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "search_Nolo_Device")]
        public  static extern void search_Nolo_Device();
        /**
          ******************************************************************************
          * Function description：Close the connection with NOLO device and release resources
          * Parameter：NULL
          * Return Value：NULL
          * Remarks：NULL
	      *【关闭与NOLO设备连接，释放资源】
          ******************************************************************************
        */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "close_Nolo_Device")]
        public  static extern void close_Nolo_Device();
	    /**
          ******************************************************************************
          * Function description：Get call-back notification when disconnect NOLO device
          * Parameter：Parameter fun ,custom function pointer
          * Return Value：Returns the function registration status:false or true
          * Remarks：NULL
	      *【断开连接NOLO设备将会回调通知】
          ******************************************************************************
        */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "registerDisConnectCallBack")]
        public static extern bool registerDisConnectCallBack(funcCallback funcCallBace , IntPtr context);
		/**
          ******************************************************************************
          * Function description：Get call-back notification when connect NOLO device
          * Parameter：Parameter fun ,custom function pointer
          * Return Value：Returns the function registration status:false or true
          * Remarks：NULL
	      *【连接NOLO设备时将会回调通知】
          ******************************************************************************
        */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "registerConnectSuccessCallBack")]
        public static extern bool registerConnectSuccessCallBack(funcCallback funcCallBace , IntPtr context);
        /**
          ******************************************************************************
          * Function description：Interface of Double click the customizable button( The Default is the menu button) or the system button to notify in real time
          * Parameter：Parameter fun ,custom function pointer
          * Return Value：Returns the function registration status:false or true
          * Remarks：无
	      *【双击自定义按键（默认为双击menu按键）或双击system按键回调通知接口函数】
          ******************************************************************************
        */
		[DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "registerExpandDataNotifyCallBack")]
        public static extern bool registerExpandDataNotifyCallBack(expandMsgFunc func , IntPtr context);
        /**
          ******************************************************************************
          * Function description：Get call-back notification whenever getting new data through USB
          * Parameter：Parameter fun ,custom function pointer
          * Return Value：Returns the function registration status:false or true
          * Remarks：NULL
	      *【从usb接收到新数据就回调通知，返回注册函数是否注册成功】
          ******************************************************************************
       */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "registerNoloDataNotifyCallBack")]
        public static extern bool registerNoloDataNotifyCallBack(noloDataNotifyFunc fun , IntPtr context);
		/**
          ******************************************************************************
          * Function description：Set vibration functions of NOLO controller
          * Parameter：Parameter devicetype is an enumeration type,Parameter intensity : Vibration intensity range from 0~100, the bigger the stronger
          * Return Value：NULL
          * Remarks：Parameter devicetype ：LeftControllerDevice、RightControllerDevice
	      *【设置NOLO手柄震动函数，参数类型为设备类型和震动强度，设备类型:左手和右手，震动强度范围0~100,值越大震动越激烈】
          ******************************************************************************
        */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "set_Nolo_TriggerHapticPulse")]
        public static extern void set_Nolo_TriggerHapticPulse(NoloDeviceType deviceType, int intensity);
		
        /*
         ******************************************************************************
         * Function description：Get all data interface function of NOLO
         * Parameter：NULL
         * Return Value：Returns the NoloData structure data
         * Remarks：NULL
	     *【获取NOLOs所有数据接口函数】
         ******************************************************************************
        */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "get_Nolo_NoloData")]
        public static extern NoloData get_Nolo_NoloData();
        /**
          ******************************************************************************
          * Function description：Get all data interface function of NOLO left controller
          * Parameter：NULL
          * Return Value：Returns the Controller structure data
          * Remarks：NULL
	      *【获取NOLO左手柄数据接口函数】
          ******************************************************************************
        */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "get_Nolo_LeftControllerData")]
        public static extern Controller get_Nolo_LeftControllerData();


        /**
          ******************************************************************************
          * Function description：Get all data interface function of NOLO right controller
          * Parameter：NULL
          * Return Value：Returns the Controller structure data
          * Remarks：NULL
	      *【获取NOLO右手柄数据接口函数】
          ******************************************************************************
        */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "get_Nolo_RightControllerData")]
        public static extern Controller get_Nolo_RightControllerData();
	
        /**
          ******************************************************************************
          * Function description：Get all data interface function of NOLO headset marker
          * Parameter：NULL
          * Return Value：Returns the HMD structure data
          * Remarks：NULL
	      *【获取NOLO头部数据接口函数】
          ******************************************************************************
        */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "get_Nolo_HMDData")]
        public static extern HMD get_Nolo_HMDData();
		/*
         ******************************************************************************
         * Function description：Get all data interface function of NOLO base station
         * Parameter：NULL 
         * Return Value：Returns the BaseStation structure data
         * Remarks：NULL
	     *【获取NOLO基站数据接口函数】
         ******************************************************************************
       */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "get_Nolo_BaseStationData")]
        public static extern BaseStation get_Nolo_BaseStationData();
		/**
          ******************************************************************************
          * Function description：Get status data of NOLO battery power based on the type of device
          * Parameter：Parameter devicetype is an enumeration type
          * Return Value：Returns the EBattery enumeration type:shut down 、low 、middle 、high
          * Remarks：NULL
	      *【根据设备类型获取NOLO设备电量数据】
          ******************************************************************************
        */
        [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "get_Nolo_Battery")]
        public static extern EBattery get_Nolo_Battery(NoloDeviceType devicetype);
    
	   /*
	    ******************************************************************************
	    * Function ：Customize the 180 degree turn-around hot key by the button types. The default is double click the menu button
	    * Parameter：Parameter keyType is an enumeration type
	    * Return Value：NULL
	    * Remarks：NULL
	    *【根据按键类型自定义旋转180热键,不进行自定义按键时默认为双击menu按键】
	    ******************************************************************************
	   */
	    [DllImportAttribute("Nolo_USBHID", CallingConvention = CallingConvention.Cdecl, EntryPoint = "set_Nolo_TurnAroundKey")]
        public static extern void set_Nolo_TurnAroundKey(EDoubleClickKeyType keyType);
	}

}
