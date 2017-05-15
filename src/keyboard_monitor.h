#ifndef OPENVR_SURVIVOR_SRC_KEYBOARD_MONITOR_H_
#define OPENVR_SURVIVOR_SRC_KEYBOARD_MONITOR_H_
#include "common.h"

class KeyBoardMonitor{
public:
	KeyBoardMonitor();
	~KeyBoardMonitor();
	DriverPose_t GetHMDPose();
	bool GetDetectKeyBoardThreadState();
	bool SetDetectKeyBoardThreadState(bool new_state);
private:
	void KeyBoardMonitor::DetectKeyBoardThread();
	std::thread m_tDetectKeyBoardThread;
	bool m_bDetectKeyBoardThreadState;//0:stop 1:running
	DriverPose_t m_sHMDPose;
};
#endif
