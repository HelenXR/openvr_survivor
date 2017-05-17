#include "keyboard_monitor.h"

KeyBoardMonitor *KeyBoardMonitor::m_pKeyBoardMonitor = NULL;
KeyBoardMonitor* KeyBoardMonitor::GetInstance(){
	if(NULL == m_pKeyBoardMonitor){
		m_pKeyBoardMonitor = new KeyBoardMonitor();
		LOG(INFO) << "new KeyBoardMonitor.";
	}
	return m_pKeyBoardMonitor;
}
KeyBoardMonitor::KeyBoardMonitor() : m_bDetectKeyBoardThreadState(true){
	DriverPose_t pose = {0};
	m_sHMDPose = pose;
	m_sHMDPose.qRotation.w = 1.0;// quaternion origin(1.0,0.0,0.0,0.0).
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
	pitch:'w' up, 's' or 'x'down.
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
#define KEYBOARD_ROTATE_STEP_DEGREE                  10
	if (KBC(VK_KEY_A)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,KEYBOARD_ROTATE_STEP_DEGREE,0,0);
		LOG(INFO) << "A Click!" << endl;
	}
	if (KBC(VK_KEY_D)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,-KEYBOARD_ROTATE_STEP_DEGREE,0,0);
		LOG(INFO) << "D Click!" << endl;
	}
	if (KBC(VK_KEY_W)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,0,KEYBOARD_ROTATE_STEP_DEGREE,0);
		LOG(INFO) << "W Click!" << endl;
	}
	if (KBC(VK_KEY_X)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,0,-KEYBOARD_ROTATE_STEP_DEGREE,0);
		LOG(INFO) << "X Click!" << endl;
	}
	if (KBC(VK_KEY_S)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation, 0, -KEYBOARD_ROTATE_STEP_DEGREE, 0);
		LOG(INFO) << "S Click!" << endl;
	}
	if (KBC(VK_KEY_Q)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,0,0,KEYBOARD_ROTATE_STEP_DEGREE);
		LOG(INFO) << "Q Click!" << endl;
	}
	if (KBC(VK_KEY_E)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,0,0,-KEYBOARD_ROTATE_STEP_DEGREE);
		LOG(INFO) << "E Click!" << endl;
	}
	if (KBC(VK_KEY_C)) {
		m_sHMDPose.qRotation = {1.0,0.0,0.0,0.0};
		LOG(INFO) << "C Click!" << endl;
	}
#endif

#if defined(HMD_POSITION_BY_KEYBOARD)
#define KEYBOARD_POSITION_STEP    0.1
	if (KBC(VK_NUMPAD8)) {
		m_sHMDPose.vecPosition[2] -= KEYBOARD_POSITION_STEP;
		LOG(INFO) << "8 Click!" << endl;
	}
	if (KBC(VK_NUMPAD2)) {
		m_sHMDPose.vecPosition[2] += KEYBOARD_POSITION_STEP;
		LOG(INFO) << "2 Click!" << endl;
	}
	if (KBC(VK_NUMPAD4)) {
		m_sHMDPose.vecPosition[0] -= KEYBOARD_POSITION_STEP;
		LOG(INFO) << "4 Click!" << endl;
	}
	if (KBC(VK_NUMPAD6)) {
		m_sHMDPose.vecPosition[0] += KEYBOARD_POSITION_STEP;
		LOG(INFO) << "6 Click!" << endl;
	}
	if (KBC(VK_NUMPAD9)) {
		m_sHMDPose.vecPosition[1] += KEYBOARD_POSITION_STEP;
		LOG(INFO) << "9 Click!" << endl;
	}
	if (KBC(VK_NUMPAD1)) {
		m_sHMDPose.vecPosition[1] -= KEYBOARD_POSITION_STEP;
		LOG(INFO) << "1 Click!" << endl;
	}
	if (KBC(VK_NUMPAD5)) {
		m_sHMDPose.vecPosition[0] = 0.0;
		m_sHMDPose.vecPosition[1] = 0.0;
		m_sHMDPose.vecPosition[2] = 0.0;
		LOG(INFO) << "5 Click!" << endl;
	}
	LOG(INFO) << "vecPosition(" << m_sHMDPose.vecPosition[0] << "," << m_sHMDPose.vecPosition[1] << "," << m_sHMDPose.vecPosition[2] << ")";
#endif
}
//yaw range:-90~90 pitch range:-180~180 roll range:-180~180.
HmdQuaternion_t KeyBoardMonitor::RotateQuaternionByYawPitchRoll(const HmdQuaternion_t quaternion_origin,double yaw_degree,double pitch_degree,double roll_degree){
	HmdQuaternion_t quaternion_dest = {0};
#if 0
	//convert quat to euler angle in degree.
	auto euler = degrees(eulerAngles(glm::quat((float)quaternion_origin.w,(float)quaternion_origin.x,(float)quaternion_origin.y,(float)quaternion_origin.z)));
	LOG(INFO) << "RotateQuaternionByYawPitchRoll:before euler(" << euler.x << "," << euler.y << "," << euler.z << ")";
	//add euler angle,pitch as x, yaw as y, roll as z.
	euler.x += (float)pitch_degree;
	euler.y += (float)yaw_degree;
	euler.z += (float)roll_degree;
	LOG(INFO) << "RotateQuaternionByYawPitchRoll:after euler(" << euler.x << "," << euler.y << "," << euler.z << ")";
	//convert euler angle in degree to quat.
	glm::quat quat = glm::quat{radians(euler)};
	quaternion_dest.x = quat.x;
	quaternion_dest.y = quat.y;
	quaternion_dest.z = quat.z;
	quaternion_dest.w = quat.w;
#else
	glm::quat quat_origin = glm::quat((float)quaternion_origin.w, (float)quaternion_origin.x, (float)quaternion_origin.y, (float)quaternion_origin.z);
	auto euler = degrees(eulerAngles(quat_origin));
	LOG(INFO) << "RotateQuaternionByYawPitchRoll:before eulerPYR(" << euler.x << "," << euler.y << "," << euler.z << ")";	
	glm::quat quat_dest;
	if(yaw_degree != 0)
		quat_dest = glm::rotate(quat_origin,glm::radians((float)yaw_degree),glm::vec3(0.f,1.f,0.f));
	else if (pitch_degree != 0)
		quat_dest = glm::rotate(quat_origin, glm::radians((float)pitch_degree), glm::vec3(1.f, 0.f, 0.f));
	else if (roll_degree != 0)
		quat_dest = glm::rotate(quat_origin, glm::radians((float)roll_degree), glm::vec3(0.f, 0.f, 1.f));
	euler = degrees(eulerAngles(quat_dest));
	LOG(INFO) << "RotateQuaternionByYawPitchRoll:after eulerPYR(" << euler.x << "," << euler.y << "," << euler.z << ")";	
	quaternion_dest.x = quat_dest.x;
	quaternion_dest.y = quat_dest.y;
	quaternion_dest.z = quat_dest.z;
	quaternion_dest.w = quat_dest.w;
#endif	
	return quaternion_dest;
}
