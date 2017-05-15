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
		LOG_EVERY_N(INFO, 10) << "DetectKeyBoardThread loop!";


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
