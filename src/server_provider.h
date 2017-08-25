/** @file 
	@brief CServerProvider implement IServerTrackedDeviceProvider.

	@date 2017.05.25
	@author helenxr(helenhololens@gmail.com)
	
	if you want,you can use this in everywhere.
	if you want to contribute to this project,join us.
*/

#ifndef OPENVR_SURVIVOR_SRC_SERVER_PROVIDER_H_
#define OPENVR_SURVIVOR_SRC_SERVER_PROVIDER_H_
#include "common.h"
#include "head_mount_display_device.h"
#include "hand_controller_device.h"

using namespace vr;
/**
	CServerProvider is requested and used in vrserver to query tracking and other information about tracked devices	
*/
class CServerProvider : public IServerTrackedDeviceProvider{
public:
	/**
		Constructor
	*/
	CServerProvider();
	/**
		Destructor
	*/
	~CServerProvider();
	/**
		initializes the driver. This will be called before any other methods are called. If Init returns anything other than HmdError_None the driver DLL will be unloaded.
		create tracked devices,like hmd and hand controller.
		@param[in] pDriverContext pointer to ctx.
		@return if init successful return VRInitError_None.
	*/
	virtual vr::EVRInitError Init( IVRDriverContext *pDriverContext ) override;
	/**
		Cleans up the driver right before it is unloaded.
	*/
	virtual void Cleanup() override;
	/**
		return all important interface version.
	*/
	virtual const char * const *GetInterfaceVersions() override;
	/**
		Allows the driver do to some work in the main loop of the server. This can be useful to avoid the need for a main thread in your driver.
	*/
	virtual void RunFrame() override;
	
	// ------------  Power State Functions ----------------------- //
	/** Returns true if the driver wants to block Standby mode. */
	virtual bool ShouldBlockStandbyMode() override;
	/** Called when the system is entering Standby mode. The driver should switch itself into whatever sort of low-power
	* state it has. */
	virtual void EnterStandby() override;
	/** Called when the system is leaving Standby mode. The driver should switch itself back to
	full operation. */
	virtual void LeaveStandby() override;
	/**
		debug by keyboard.
	*/
	void KeyBoardDebugFunction();
	/**
		init six dof module sdk.
	*/
	void SixDofTrackingModuleInit();
	/**
		set six dof module  e.g:nolo ximmerse.
	*/
	void SetSixDofModuleType(ESixDofTrackingModule six_dof_module);
	/**
		MultiLightWeightFunctionThread.
		handle light weight function, e.g keyboard debug,glog log flush ,six dof detect etc.
	*/
	void MultiLightWeightFunctionThread();	
	/**
		update six dof module state.
	*/
	void UpdateSixDofModuleState();
	/**
		update six dof module tracking data.
	*/
	void UpdateSixDofModuleTrackingData();	
 	/**
		HandleTrackedDevicePostMessageThread.
	*/
	void HandleTrackedDevicePostMessageThread();	
	#ifdef USE_NOLO_SIX_DOF_TRACKING_MODULE
	/**
		when hmd tracking module connected,this function will be call.
	*/
	static void NoloHMDModuleConnect(void *context);
	/**
		when hmd tracking module disconnected,this function will be call
	*/
	static void NoloHMDModuleDisconnect(void *context);
	/**
		when nolo data(hmd or controllers) data is ready,this function will be call.
	*/
	static void NoloDataNotify(NOLO::NoloData noloData,void *context);
	/**
		when nolo expand msg (e.g double click Home/System) notify,this function will be call.
	*/
	static void NoloExpandDataNotify(NOLO::ExpandMsgType expandMsgType , void *context);
	#endif	
private:
	
	CHeadMountDisplayDevice *m_pHeadMountDisplay;///< pointer to HMD
	CHandControllerDevice *m_pHandControllerDevice[HAND_CONTROLLER_COUNT];///< pointer to hand controllers
#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
	int m_nXimmerseHandle[XIMMERSE_DEVICE_MAX] = {-1,-1,-1,-1};	//< 0:XHawk-0  1:XCobra-0 2:XCobra-1 3:VRDevice
#endif	
	ESixDofTrackingModule m_eSixDofTrackingModule;				//< six dof tracking module type.
	bool m_bMultiLightWeightFunctionThreadState;			//<false:stop true:running.
	std::thread m_tMultiLightWeightFunctionThread; //< Multi Light Weight Function Thread.
	bool m_bControllerState[HAND_CONTROLLER_COUNT];								//< true:controller is connect  false: controller is disconnect
	std::thread m_tHandleTrackedDevicePostMessageThread;		//<handle all tracked devices' message;
	bool m_bHandleTrackedDevicePostMessageThreadState;			//<false:stop true:running.
	uint64_t m_uHandleTrackedDevicePostMessageThreadID;			//< HandleTrackedDevicePostMessageThread id;	
	static	CServerProvider *m_pThis;							//< use to static member pointer this.
};

#endif
