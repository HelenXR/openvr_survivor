#ifndef OPENVR_SURVIVOR_SRC_KEYBOARD_MONITOR_H_
#define OPENVR_SURVIVOR_SRC_KEYBOARD_MONITOR_H_
#include "common.h"
#include <windows.h>
#include "virtual_key_codes.h"
#include <gtc/quaternion.hpp>

//whether keyboard is Click.Virtual-Key Codes comes from MSDN(https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx)
#define KBC(x)               (GetAsyncKeyState(x) & 0x0001)
class KeyBoardMonitor{
public:
	static KeyBoardMonitor* GetInstance();
	~KeyBoardMonitor();
	DriverPose_t GetHMDPose();
	bool GetDetectKeyBoardThreadState();
	bool SetDetectKeyBoardThreadState(bool new_state);
protected:	
	KeyBoardMonitor();
private:
	void DetectKeyBoardThread();
	void KeyBoardForHMDPoseUpdate();
	HmdQuaternion_t RotateQuaternionByYawPitchRoll(const HmdQuaternion_t quaternion_origin,double yaw_degree,double pitch_degree,double roll_degree);
	std::thread m_tDetectKeyBoardThread;
	bool m_bDetectKeyBoardThreadState;//0:stop 1:running
	DriverPose_t m_sHMDPose;
	static KeyBoardMonitor *m_pKeyBoardMonitor;
};
#endif
