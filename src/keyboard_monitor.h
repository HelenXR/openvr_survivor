#ifndef OPENVR_SURVIVOR_SRC_KEYBOARD_MONITOR_H_
#define OPENVR_SURVIVOR_SRC_KEYBOARD_MONITOR_H_
#include "common.h"
#include <windows.h>
#include "virtual_key_codes.h"

//whether keyboard is Click.Virtual-Key Codes comes from MSDN(https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx)
#define KBC(x)               (GetAsyncKeyState(x) & 0x0001)
class KeyBoardMonitor{
public:
	KeyBoardMonitor();
	~KeyBoardMonitor();
	DriverPose_t GetHMDPose();
	bool GetDetectKeyBoardThreadState();
	bool SetDetectKeyBoardThreadState(bool new_state);
private:
	void DetectKeyBoardThread();
	void KeyBoardForHMDPoseUpdate();
	std::thread m_tDetectKeyBoardThread;
	bool m_bDetectKeyBoardThreadState;//0:stop 1:running
	DriverPose_t m_sHMDPose;
};
#endif
