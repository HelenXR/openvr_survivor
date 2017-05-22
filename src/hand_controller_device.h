#ifndef OPENVR_SURVIVOR_SRC_HAND_CONTROLLER_DEVICE_H_
#define OPENVR_SURVIVOR_SRC_HAND_CONTROLLER_DEVICE_H_
#include "common.h"
#include <string>
#include <thread>
#include "keyboard_monitor.h"

typedef void (vr::IVRServerDriverHost::*ButtonUpdate)(uint32_t unWhichDevice, vr::EVRButtonId eButtonId, double eventTimeOffset);
class CHandControllerDevice : public ITrackedDeviceServerDriver,public IVRControllerComponent{
public:
	CHandControllerDevice(string serial_number,ETrackedControllerRole controller_role);
	~CHandControllerDevice();
	
	//implement ITrackedDeviceServerDriver
	EVRInitError Activate( uint32_t unObjectId ) override;
	void Deactivate() override;
	void EnterStandby() override;
	void *GetComponent( const char *pchComponentNameAndVersion ) override;
	void DebugRequest( const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize ) override;
	DriverPose_t GetPose() override;	
	
	//implement IVRControllerComponent		
	VRControllerState_t GetControllerState( ) override;
	bool TriggerHapticPulse( uint32_t unAxisId, uint16_t usPulseDurationMicroseconds ) override;
	
	//custom define function
	const char *GetSerialNumber();
	const uint32_t GetUniqueObjectId();
private:
	void ReportPoseButtonThread();
	void GetButtonState(KeyBoardForControllerButton button_state);
	void SendButtonUpdates(ButtonUpdate ButtonEvent, uint64_t ulMask);
	string m_sSerialNumber;
	uint32_t m_nUniqueObjectId;
	DriverPose_t m_Pose;
	EHandController m_eHandController;
	vr::VRControllerState_t m_ControllerState;
	vr::PropertyContainerHandle_t m_PropertyContainerHandle;
	std::thread m_tReportPoseButtonThread;
	bool m_bReportPoseButtonThreadState;                //false:stop true:running.
#if defined(CONTROLLER_ROTATE_BY_KEYBOARD) || defined(CONTROLLER_POSITION_BY_KEYBOARD)
	KeyBoardMonitor *m_pKeyBoardMonitor;
#endif	
};

#endif
