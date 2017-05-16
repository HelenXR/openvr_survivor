#include "keyboard_monitor.h"

KeyBoardMonitor::KeyBoardMonitor() : m_bDetectKeyBoardThreadState(true){
	DriverPose_t pose = {0};
	m_sHMDPose = pose;
	LOG(INFO) << "KeyBoardMonitor Construct!";
	//create thread for detect keyboard input.
	m_tDetectKeyBoardThread = std::thread(&KeyBoardMonitor::DetectKeyBoardThread, this);
}

KeyBoardMonitor::~KeyBoardMonitor(){
	//clean thread & sources.
	m_bDetectKeyBoardThreadState = false;
}

DriverPose_t KeyBoardMonitor::GetHMDPose(){
	return m_sHMDPose;
}

void KeyBoardMonitor::DetectKeyBoardThread() {
	LOG(INFO) << "DetectKeyBoardThread:run!";
	auto retryInterval = std::chrono::milliseconds(100);
	auto pollDeadline = std::chrono::steady_clock::now();

	while (m_bDetectKeyBoardThreadState) {
		LOG_EVERY_N(INFO, 10*10) << "DetectKeyBoardThread loop!";

		//keyboard for HMD
		KeyBoardForHMDPoseUpdate();
		pollDeadline += retryInterval;
		std::this_thread::sleep_until(pollDeadline);
	}
	LOG(INFO) << "DetectKeyBoardThread:exit!";
}

bool KeyBoardMonitor::GetDetectKeyBoardThreadState() {
	return m_bDetectKeyBoardThreadState;
}

bool KeyBoardMonitor::SetDetectKeyBoardThreadState(bool new_state) {
	m_bDetectKeyBoardThreadState = new_state;
	return true;
}
/*
rotate information:
	yaw:'a'  left, 'd' right.
	pitch:'w' up, 's' down.
	roll:'q' anticlockwise,'c' clockwise.
	other function:'c' clear rotate.
position information:
    forward:'8',back:'2'.
	right:'6',left:'4'.
	up:'9',down:'1'.
	other function:'5' clear position.
	note:The numeric key above is the numeric key on the right.
*/
void KeyBoardMonitor::KeyBoardForHMDPoseUpdate(){

#if defined(HMD_ROTATE_BY_KEYBOARD)
	if (KBC(VK_KEY_A)) {
		LOG(INFO) << "A Click!" << endl;
	}
	if (KBC(VK_KEY_D)) {
		LOG(INFO) << "D Click!" << endl;
	}
	if (KBC(VK_KEY_W)) {
		LOG(INFO) << "W Click!" << endl;
	}
	if (KBC(VK_KEY_S)) {
		LOG(INFO) << "S Click!" << endl;
	}
	if (KBC(VK_KEY_Q)) {
		LOG(INFO) << "Q Click!" << endl;
	}
	if (KBC(VK_KEY_E)) {
		LOG(INFO) << "E Click!" << endl;
	}
	if (KBC(VK_KEY_C)) {
		LOG(INFO) << "C Click!" << endl;
	}
#endif

#if defined(HMD_POSITION_BY_KEYBOARD)
	if (KBC(VK_NUMPAD8)) {
		LOG(INFO) << "8 Click!" << endl;
	}
	if (KBC(VK_NUMPAD2)) {
		LOG(INFO) << "2 Click!" << endl;
	}
	if (KBC(VK_NUMPAD4)) {
		LOG(INFO) << "4 Click!" << endl;
	}
	if (KBC(VK_NUMPAD6)) {
		LOG(INFO) << "6 Click!" << endl;
	}
	if (KBC(VK_NUMPAD9)) {
		LOG(INFO) << "9 Click!" << endl;
	}
	if (KBC(VK_NUMPAD1)) {
		LOG(INFO) << "1 Click!" << endl;
	}
	if (KBC(VK_NUMPAD5)) {
		LOG(INFO) << "5 Click!" << endl;
	}
#endif
}
