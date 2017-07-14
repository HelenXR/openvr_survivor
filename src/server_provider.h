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
	void CServerProvider::KeyBoardDebugFunction();
private:
	
	CHeadMountDisplayDevice *m_pHeadMountDisplay;///< pointer to HMD
	CHandControllerDevice *m_pHandControllerDevice[HAND_CONTROLLER_COUNT];///< pointer to hand controllers
};

#endif
