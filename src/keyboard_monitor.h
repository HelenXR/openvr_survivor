#ifndef OPENVR_SURVIVOR_SRC_KEYBOARD_MONITOR_H_
#define OPENVR_SURVIVOR_SRC_KEYBOARD_MONITOR_H_
#include "common.h"
#include <windows.h>
#include "virtual_key_codes.h"

//whether keyboard is Click.Virtual-Key Codes comes from MSDN(https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx)
#define KBC(x)               (GetAsyncKeyState(x) & 0x0001)
//whether keyboard is Touch.Virtual-Key Codes comes from MSDN(https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx)
#define KBT(x)               (GetAsyncKeyState(x) & 0x8000)
class KeyBoardMonitor{
public:
	static KeyBoardMonitor* GetInstance();
	~KeyBoardMonitor();
	DriverPose_t GetHMDPose();
	DriverPose_t GetControllerPose(EHandController hand_controller);
	KeyBoardForControllerButton GetControllerButtonState(EHandController hand_controller);
	bool GetDetectKeyBoardThreadState();
	bool SetDetectKeyBoardThreadState(bool new_state);
protected:	
	KeyBoardMonitor();//singleton pattern.
private:
	void DetectKeyBoardThread();
	void KeyBoardForHMDPoseUpdate();
	void KeyBoardForControllerPoseAndButtonUpdate();
	HmdQuaternion_t RotateQuaternionByYawPitchRoll(const HmdQuaternion_t quaternion_origin,double yaw_degree,double pitch_degree,double roll_degree);
	std::thread m_tDetectKeyBoardThread;
	bool m_bDetectKeyBoardThreadState;//0:stop 1:running
	DriverPose_t m_sHMDPose;
	DriverPose_t m_sControllerPose[HAND_CONTROLLER_COUNT];
	static KeyBoardMonitor *m_pKeyBoardMonitor;
	KeyBoardForControllerButton m_KeyBoardForControllerButton[HAND_CONTROLLER_COUNT];
};
#endif
