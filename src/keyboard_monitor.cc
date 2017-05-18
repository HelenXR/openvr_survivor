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
	LOG(INFO) << "KeyBoardMonitor Construct!";
	DriverPose_t pose = {0};
	//init HMD's pose.
	m_sHMDPose = pose;
	m_sHMDPose.qRotation.w = 1.0;// quaternion origin(1.0,0.0,0.0,0.0).

	//init left/right controller's pose position.
	m_sControllerPose[LEFT_HAND_CONTROLLER] = pose;
	m_sControllerPose[LEFT_HAND_CONTROLLER].qRotation.w = 1.0;// quaternion origin(1.0,0.0,0.0,0.0).
	m_sControllerPose[LEFT_HAND_CONTROLLER].vecPosition[0] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_X;
	m_sControllerPose[LEFT_HAND_CONTROLLER].vecPosition[1] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_Y;
	m_sControllerPose[LEFT_HAND_CONTROLLER].vecPosition[2] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_Z;
	m_sControllerPose[RIGHT_HAND_CONTROLLER] = pose;
	m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation.w = 1.0;// quaternion origin(1.0,0.0,0.0,0.0).
	m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[0] = HAND_CONTROLLER_RELATIVE_HMD_POSITION_X;
	m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[1] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_Y;
	m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[2] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_Z;
	
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

DriverPose_t KeyBoardMonitor::GetControllerPose(EHandController hand_controller) {
	LOG_IF(ERROR, hand_controller >= HAND_CONTROLLER_COUNT);
	return m_sControllerPose[hand_controller];
}

void KeyBoardMonitor::DetectKeyBoardThread() {
	LOG(INFO) << "DetectKeyBoardThread:run!";
	auto retryInterval = std::chrono::milliseconds(100);
	auto pollDeadline = std::chrono::steady_clock::now();

	while (m_bDetectKeyBoardThreadState) {
		LOG_EVERY_N(INFO, 10*10) << "DetectKeyBoardThread loop!";

		//keyboard for HMD
		KeyBoardForHMDPoseUpdate();

		//keyboard for controller
		KeyBoardForControllerPoseAndButtonUpdate();
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
    forward:'VK_NUMPAD8',back:'VK_NUMPAD2'.
	right:'VK_NUMPAD6',left:'VK_NUMPAD4'.
	up:'VK_NUMPAD9',down:'VK_NUMPAD1'.
	other function:'VK_NUMPAD5' clear position.
	note:The numeric key above is the numeric key on the right.
*/
void KeyBoardMonitor::KeyBoardForHMDPoseUpdate(){

#if defined(HMD_ROTATE_BY_KEYBOARD)
#define KEYBOARD_FOR_HMD_ROTATE_STEP_DEGREE                  10
	if (KBC(VK_KEY_A)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation, KEYBOARD_FOR_HMD_ROTATE_STEP_DEGREE,0,0);
		LOG(INFO) << "A Click!" << endl;
	}
	if (KBC(VK_KEY_D)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,-KEYBOARD_FOR_HMD_ROTATE_STEP_DEGREE,0,0);
		LOG(INFO) << "D Click!" << endl;
	}
	if (KBC(VK_KEY_W)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,0, KEYBOARD_FOR_HMD_ROTATE_STEP_DEGREE,0);
		LOG(INFO) << "W Click!" << endl;
	}
	if (KBC(VK_KEY_X)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,0,-KEYBOARD_FOR_HMD_ROTATE_STEP_DEGREE,0);
		LOG(INFO) << "X Click!" << endl;
	}
	if (KBC(VK_KEY_S)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation, 0, -KEYBOARD_FOR_HMD_ROTATE_STEP_DEGREE, 0);
		LOG(INFO) << "S Click!" << endl;
	}
	if (KBC(VK_KEY_Q)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,0,0, KEYBOARD_FOR_HMD_ROTATE_STEP_DEGREE);
		LOG(INFO) << "Q Click!" << endl;
	}
	if (KBC(VK_KEY_E)) {
		m_sHMDPose.qRotation = RotateQuaternionByYawPitchRoll(m_sHMDPose.qRotation,0,0,-KEYBOARD_FOR_HMD_ROTATE_STEP_DEGREE);
		LOG(INFO) << "E Click!" << endl;
	}
	if (KBC(VK_KEY_C)) {
		m_sHMDPose.qRotation = {1.0,0.0,0.0,0.0};
		LOG(INFO) << "C Click!" << endl;
	}
#endif

#if defined(HMD_POSITION_BY_KEYBOARD)
#define KEYBOARD_FOR_HMD_POSITION_STEP    0.1
	if (KBC(VK_NUMPAD8)) {
		m_sHMDPose.vecPosition[2] -= KEYBOARD_FOR_HMD_POSITION_STEP;
		LOG(INFO) << "8 Click!" << endl;
	}
	if (KBC(VK_NUMPAD2)) {
		m_sHMDPose.vecPosition[2] += KEYBOARD_FOR_HMD_POSITION_STEP;
		LOG(INFO) << "2 Click!" << endl;
	}
	if (KBC(VK_NUMPAD4)) {
		m_sHMDPose.vecPosition[0] -= KEYBOARD_FOR_HMD_POSITION_STEP;
		LOG(INFO) << "4 Click!" << endl;
	}
	if (KBC(VK_NUMPAD6)) {
		m_sHMDPose.vecPosition[0] += KEYBOARD_FOR_HMD_POSITION_STEP;
		LOG(INFO) << "6 Click!" << endl;
	}
	if (KBC(VK_NUMPAD9)) {
		m_sHMDPose.vecPosition[1] += KEYBOARD_FOR_HMD_POSITION_STEP;
		LOG(INFO) << "9 Click!" << endl;
	}
	if (KBC(VK_NUMPAD1)) {
		m_sHMDPose.vecPosition[1] -= KEYBOARD_FOR_HMD_POSITION_STEP;
		LOG(INFO) << "1 Click!" << endl;
	}
	if (KBC(VK_NUMPAD5)) {
		m_sHMDPose.vecPosition[0] = 0.0;
		m_sHMDPose.vecPosition[1] = 0.0;
		m_sHMDPose.vecPosition[2] = 0.0;
		LOG(INFO) << "5 Click!" << endl;
	}
	LOG_EVERY_N(INFO,10*10) << "HMD vecPosition(" << m_sHMDPose.vecPosition[0] << "," << m_sHMDPose.vecPosition[1] << "," << m_sHMDPose.vecPosition[2] << ")";
#endif
}

/*
right controller rotate information:
yaw:'j'  left, 'l' right.
pitch:'i' up, 'k' or ',' down.
roll:'u' anticlockwise,'o' clockwise.
other function:'.' clear rotate.
right controller position information:
forward:'VK_UP',back:'VK_DOWN'.
right:'VK_RIGHT',left:'VK_LEFT'.
up:'VK_NUMPAD0',down:'VK_RCONTROL'.
other function:'VK_DECIMAL' clear position.
*/
void KeyBoardMonitor::KeyBoardForControllerPoseAndButtonUpdate() {

	//LEFT HAND pose update
	//......future will fill in.

	//RIGHT HAND pose update
#if defined(CONTROLLER_ROTATE_BY_KEYBOARD)
#define KEYBOARD_FOR_CONTROLLER_ROTATE_STEP_DEGREE                  10
	if (KBC(VK_KEY_J)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation = RotateQuaternionByYawPitchRoll(m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation, KEYBOARD_FOR_CONTROLLER_ROTATE_STEP_DEGREE, 0, 0);
		LOG(INFO) << "J Click!" << endl;
	}
	if (KBC(VK_KEY_L)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation = RotateQuaternionByYawPitchRoll(m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation, -KEYBOARD_FOR_CONTROLLER_ROTATE_STEP_DEGREE, 0, 0);
		LOG(INFO) << "L Click!" << endl;
	}
	if (KBC(VK_KEY_I)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation = RotateQuaternionByYawPitchRoll(m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation, 0, KEYBOARD_FOR_CONTROLLER_ROTATE_STEP_DEGREE, 0);
		LOG(INFO) << "I Click!" << endl;
	}
	if (KBC(VK_KEY_K)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation = RotateQuaternionByYawPitchRoll(m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation, 0, -KEYBOARD_FOR_CONTROLLER_ROTATE_STEP_DEGREE, 0);
		LOG(INFO) << "K Click!" << endl;
	}
	if (KBC(VK_OEM_COMMA)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation = RotateQuaternionByYawPitchRoll(m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation, 0, -KEYBOARD_FOR_CONTROLLER_ROTATE_STEP_DEGREE, 0);
		LOG(INFO) << ", Click!" << endl;
	}
	if (KBC(VK_KEY_U)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation = RotateQuaternionByYawPitchRoll(m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation, 0, 0, KEYBOARD_FOR_CONTROLLER_ROTATE_STEP_DEGREE);
		LOG(INFO) << "U Click!" << endl;
	}
	if (KBC(VK_KEY_O)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation = RotateQuaternionByYawPitchRoll(m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation, 0, 0, -KEYBOARD_FOR_CONTROLLER_ROTATE_STEP_DEGREE);
		LOG(INFO) << "O Click!" << endl;
	}
	if (KBC(VK_OEM_PERIOD)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].qRotation = { 1.0,0.0,0.0,0.0 };
		LOG(INFO) << ". Click!" << endl;
	}
#endif

#if defined(CONTROLLER_POSITION_BY_KEYBOARD)
#define KEYBOARD_FOR_CONTROLLER_POSITION_STEP    0.1
	if (KBC(VK_UP)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[2] -= KEYBOARD_FOR_CONTROLLER_POSITION_STEP;
		LOG(INFO) << "VK_UP Click!" << endl;
	}
	if (KBC(VK_DOWN)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[2] += KEYBOARD_FOR_CONTROLLER_POSITION_STEP;
		LOG(INFO) << "VK_DOWN Click!" << endl;
	}
	if (KBC(VK_LEFT)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[0] -= KEYBOARD_FOR_CONTROLLER_POSITION_STEP;
		LOG(INFO) << "VK_LEFT Click!" << endl;
	}
	if (KBC(VK_RIGHT)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[0] += KEYBOARD_FOR_CONTROLLER_POSITION_STEP;
		LOG(INFO) << "VK_RIGHT Click!" << endl;
	}
	if (KBC(VK_NUMPAD0)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[1] += KEYBOARD_FOR_CONTROLLER_POSITION_STEP;
		LOG(INFO) << "VK_NUMPAD0 Click!" << endl;
	}
	if (KBC(VK_RCONTROL)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[1] -= KEYBOARD_FOR_CONTROLLER_POSITION_STEP;
		LOG(INFO) << "1 Click!" << endl;
	}
	if (KBC(VK_DECIMAL)) {
		m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[0] = 0.2;
		m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[1] = -0.2;
		m_sControllerPose[RIGHT_HAND_CONTROLLER].vecPosition[2] = -0.5;
		LOG(INFO) << "VK_DECIMAL Click!" << endl;
	}
	LOG_EVERY_N(INFO, 10 * 10) << "Controller vecPosition(" << m_sHMDPose.vecPosition[0] << "," << m_sHMDPose.vecPosition[1] << "," << m_sHMDPose.vecPosition[2] << ")";
#endif
	//button update
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
