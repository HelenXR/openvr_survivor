#include "hand_controller_device.h"

CHandControllerDevice::CHandControllerDevice(string serial_number,ETrackedControllerRole controller_role){
	LOG(INFO) << "CHandControllerDevice:" << serial_number;
	m_sSerialNumber = serial_number;

	//init m_pose struct
	memset( &m_Pose, 0, sizeof( m_Pose ) );
	m_Pose.willDriftInYaw = true;
	m_Pose.shouldApplyHeadModel = false;
	#if defined(CONTROLLER_ROTATE_BY_KEYBOARD) && defined(CONTROLLER_POSITION_BY_KEYBOARD)
		m_Pose.deviceIsConnected = true;
		m_Pose.poseIsValid = true;
	#else
		m_Pose.deviceIsConnected = false;
		m_Pose.poseIsValid = false;
	#endif
	m_Pose.result = ETrackingResult::TrackingResult_Running_OK;
	m_Pose.qRotation = HmdQuaternion_Init( 1, 0, 0, 0 );
	m_Pose.qWorldFromDriverRotation = HmdQuaternion_Init( 1, 0, 0, 0 );
	m_Pose.qDriverFromHeadRotation = HmdQuaternion_Init( 1,0, 0, 0);
	m_Pose.poseTimeOffset = -0.016f;	
	switch (controller_role) {
		case TrackedControllerRole_LeftHand:
			m_Pose.vecPosition[0] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_X;
			m_Pose.vecPosition[1] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_Y;
			m_Pose.vecPosition[2] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_Z;
			m_eHandController = LEFT_HAND_CONTROLLER;
			m_cControllerRole = 'L';
			break;
		case TrackedControllerRole_RightHand:
			m_Pose.vecPosition[0] = HAND_CONTROLLER_RELATIVE_HMD_POSITION_X;
			m_Pose.vecPosition[1] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_Y;
			m_Pose.vecPosition[2] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_Z;
			m_eHandController = RIGHT_HAND_CONTROLLER;
			m_cControllerRole = 'R';
			break;
		default:
			LOG(ERROR) << "CHandControllerDevice:" << controller_role;
	}
	m_fHmdXPositionOffset = vr::VRSettings()->GetFloat( k_pch_Sample_Section, k_pch_Sample_HmdXPositionOffset_Float );
	m_fHmdYPositionOffset = vr::VRSettings()->GetFloat( k_pch_Sample_Section, k_pch_Sample_HmdYPositionOffset_Float );
	m_fHmdZPositionOffset = vr::VRSettings()->GetFloat( k_pch_Sample_Section, k_pch_Sample_HmdZPositionOffset_Float );
    m_bTopCamera = vr::VRSettings()->GetBool( k_pch_Sample_Section, k_pch_Sample_TopCamera );
    m_bCameraHeight = vr::VRSettings()->GetFloat( k_pch_Sample_Section, k_pch_Sample_CameraHeight );
	m_eSixModuleType = NONE_SIX_DOF_TRACKING_MODULE;
	m_nReportPoseInterval = std::chrono::milliseconds( NONE_SIX_DOF_CONTROLLER_POSE_REPORT_INTERVAL );
#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
	m_nXimmerseXCobraHandle = -1;
	m_uButtonPressCurrentTime = 0;
	m_uButtonPressLastTime = 0;
	m_uButton2PressCurrentTime = 0;
	m_uButton2PressLastTime = 0;	
#endif	
	m_uServerProviderHandleTrackedDevicePostMessageThreadID = -1;
	m_bTrunAround = false;
	
	//init m_ControllerState,m_PropertyContainerHandle
	memset( &m_ControllerState, 0, sizeof( m_ControllerState ) );
	m_PropertyContainerHandle = vr::k_ulInvalidPropertyContainer;
	
	//create report controller pose and button thread
	m_tReportPoseButtonThread = std::thread(&CHandControllerDevice::ReportPoseButtonThread ,this);
	m_bReportPoseButtonThreadState = true;	
#if defined(CONTROLLER_ROTATE_BY_KEYBOARD) || defined(CONTROLLER_POSITION_BY_KEYBOARD) 
	m_pKeyBoardMonitor = KeyBoardMonitor::GetInstance();
#endif
}

CHandControllerDevice::~CHandControllerDevice(){

}

EVRInitError CHandControllerDevice::Activate( uint32_t unObjectId ){
	uint64_t u64_parameter;
	m_nUniqueObjectId = unObjectId;
	m_PropertyContainerHandle = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_nUniqueObjectId);

	//set controller's display to htc.
	vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_ModelNumber_String, "ViveMV");
	vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_ManufacturerName_String, "HTC");
	vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_RenderModelName_String, "vr_controller_vive_1_5");
	//set general properties
	vr::VRProperties()->SetStringProperty( m_PropertyContainerHandle, Prop_TrackingSystemName_String, "Survivor's Controller" );
	vr::VRProperties()->SetInt32Property( m_PropertyContainerHandle, Prop_DeviceClass_Int32, TrackedDeviceClass_Controller);
	vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, Prop_SerialNumber_String, m_sSerialNumber.c_str());
	// set Properties that are unique to TrackedDeviceClass_Controller
	vr::VRProperties()->SetInt32Property(m_PropertyContainerHandle, Prop_Axis0Type_Int32, k_eControllerAxis_TrackPad);
	vr::VRProperties()->SetInt32Property(m_PropertyContainerHandle, Prop_Axis1Type_Int32, k_eControllerAxis_Trigger);
	u64_parameter = vr::ButtonMaskFromId(vr::k_EButton_System) |
		vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger) |
		vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad) |
		vr::ButtonMaskFromId(vr::k_EButton_A) |
		vr::ButtonMaskFromId(k_EButton_DPad_Left) |
		vr::ButtonMaskFromId(k_EButton_DPad_Up) |
		vr::ButtonMaskFromId(k_EButton_DPad_Right) |
		vr::ButtonMaskFromId(k_EButton_DPad_Down) |
		vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu) |
		vr::ButtonMaskFromId(vr::k_EButton_Grip);
	vr::VRProperties()->SetUint64Property(m_PropertyContainerHandle, Prop_SupportedButtons_Uint64, u64_parameter);
	// Set icons,you can select yourself icon.
	switch (m_eHandController)
	{
	case LEFT_HAND_CONTROLLER:	
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceOff_String, "{openvr_survivor}/icons/left_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceSearching_String, "{openvr_survivor}/icons/left_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceSearchingAlert_String, "{openvr_survivor}/icons/left_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceReady_String, "{openvr_survivor}/icons/left_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceReadyAlert_String, "{openvr_survivor}/icons/left_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceNotReady_String, "{openvr_survivor}/icons/left_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceStandby_String, "{openvr_survivor}/icons/left_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceAlertLow_String, "{openvr_survivor}/icons/left_hand_controller_icon.png");
		break;
	case RIGHT_HAND_CONTROLLER:
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceOff_String, "{openvr_survivor}/icons/right_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceSearching_String, "{openvr_survivor}/icons/right_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceSearchingAlert_String, "{openvr_survivor}/icons/right_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceReady_String, "{openvr_survivor}/icons/right_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceReadyAlert_String, "{openvr_survivor}/icons/right_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceNotReady_String, "{openvr_survivor}/icons/right_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceStandby_String, "{openvr_survivor}/icons/right_hand_controller_icon.png");
		vr::VRProperties()->SetStringProperty(m_PropertyContainerHandle, vr::Prop_NamedIconPathDeviceAlertLow_String, "{openvr_survivor}/icons/right_hand_controller_icon.png");
		break;
	default:
		break;
	}

	return vr::VRInitError_None;
}

void CHandControllerDevice::Deactivate(){
	m_nUniqueObjectId = k_unTrackedDeviceIndexInvalid;
	m_bReportPoseButtonThreadState = false;
	m_tReportPoseButtonThread.join();
}
void CHandControllerDevice::EnterStandby(){

}
void *CHandControllerDevice::GetComponent( const char *pchComponentNameAndVersion ){
	void *p = NULL;
	return p;
}
void CHandControllerDevice::DebugRequest( const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize ){

}

DriverPose_t CHandControllerDevice::GetPose(){
	//get controller pose.
#if defined(CONTROLLER_ROTATE_BY_KEYBOARD)
	m_Pose.qRotation = m_pKeyBoardMonitor->GetControllerPose(m_eHandController).qRotation;
#endif

#if defined(CONTROLLER_POSITION_BY_KEYBOARD)
	double own_position[3];
	own_position[0] = m_pKeyBoardMonitor->GetControllerPose(m_eHandController).vecPosition[0];
	own_position[1] = m_pKeyBoardMonitor->GetControllerPose(m_eHandController).vecPosition[1];
	own_position[2] = m_pKeyBoardMonitor->GetControllerPose(m_eHandController).vecPosition[2];

	//before report,must adapte to HMD!
	DriverPose_t hmd_pose = m_pKeyBoardMonitor->GetHMDPose();
	m_Pose.qRotation = glm_adapter::QuaternionMultiplyQuaternion(hmd_pose.qRotation,m_Pose.qRotation);

	//hand position = hmd_position + default_relative_position + own_position
	m_Pose.vecPosition[0] = hmd_pose.vecPosition[0];
	m_Pose.vecPosition[1] = hmd_pose.vecPosition[1];
	m_Pose.vecPosition[2] = hmd_pose.vecPosition[2];
	double left_hand_default_relative_position[3] =  { -HAND_CONTROLLER_RELATIVE_HMD_POSITION_X,-HAND_CONTROLLER_RELATIVE_HMD_POSITION_Y,-HAND_CONTROLLER_RELATIVE_HMD_POSITION_Z };
	double right_hand_default_relative_position[3] = {  HAND_CONTROLLER_RELATIVE_HMD_POSITION_X,-HAND_CONTROLLER_RELATIVE_HMD_POSITION_Y,-HAND_CONTROLLER_RELATIVE_HMD_POSITION_Z };
	switch(m_eHandController){
		case LEFT_HAND_CONTROLLER:
			glm_adapter::QuaternionMultiplyDouble3(hmd_pose.qRotation, left_hand_default_relative_position,left_hand_default_relative_position);
			m_Pose.vecPosition[0] += left_hand_default_relative_position[0];
			m_Pose.vecPosition[1] += left_hand_default_relative_position[1];
			m_Pose.vecPosition[2] += left_hand_default_relative_position[2];		
			break;
		case RIGHT_HAND_CONTROLLER:			
			glm_adapter::QuaternionMultiplyDouble3(hmd_pose.qRotation, right_hand_default_relative_position,right_hand_default_relative_position);
			m_Pose.vecPosition[0] += right_hand_default_relative_position[0];
			m_Pose.vecPosition[1] += right_hand_default_relative_position[1];
			m_Pose.vecPosition[2] += right_hand_default_relative_position[2];		
			break;
	}	

	glm_adapter::QuaternionMultiplyDouble3(hmd_pose.qRotation, own_position,own_position);
	m_Pose.vecPosition[0] += own_position[0];
	m_Pose.vecPosition[1] += own_position[1];
	m_Pose.vecPosition[2] += own_position[2];
#endif

	return m_Pose;
}

VRControllerState_t CHandControllerDevice::GetControllerState( ){
	LOG(INFO) << __FUNCTION__;
	m_ControllerState.unPacketNum++;
	return m_ControllerState;
}
bool CHandControllerDevice::TriggerHapticPulse( uint32_t unAxisId, uint16_t usPulseDurationMicroseconds ){
	LOG(INFO) << __FUNCTION__;
#ifdef USE_NOLO_SIX_DOF_TRACKING_MODULE
	if(m_eSixModuleType == NOLO_SIX_DOF_TRACKING_MODULE){
		int intensity = usPulseDurationMicroseconds/40;	
		if (intensity>50) {	
			intensity = 50;
		}
		if(m_cControllerRole == 'L'){
			NOLO::set_Nolo_TriggerHapticPulse(NOLO::LeftControllerDevice ,intensity + 50);
		}else{
			NOLO::set_Nolo_TriggerHapticPulse(NOLO::RightControllerDevice ,intensity + 50);
		}

	}
#endif
	return true;
}

const char *CHandControllerDevice::GetSerialNumber(){
	LOG(INFO) << __FUNCTION__;
	return m_sSerialNumber.c_str();
}

const uint32_t CHandControllerDevice::GetUniqueObjectId(){
	LOG(INFO) << __FUNCTION__;
    return m_nUniqueObjectId;
}

void CHandControllerDevice::ReportPoseButtonThread(){
	Sleep(1*1000);
	auto pollDeadline = std::chrono::steady_clock::now();	
	LOG(INFO) << "ReportPoseButtonThread:run,interval=" << m_nReportPoseInterval.count();
	while(m_bReportPoseButtonThreadState){
		
		//update pose
		if ( m_nUniqueObjectId != vr::k_unTrackedDeviceIndexInvalid )
		{
			vr::VRServerDriverHost()->TrackedDevicePoseUpdated( m_nUniqueObjectId, GetPose(), sizeof( DriverPose_t ) );
		}
		//update button state
#if defined(CONTROLLER_ROTATE_BY_KEYBOARD) || defined(CONTROLLER_POSITION_BY_KEYBOARD)
		GetButtonState(m_pKeyBoardMonitor->GetControllerButtonState(m_eHandController));
#endif
		//LOG_EVERY_N(INFO,60*10) << "controller pose button loop!" ;
		pollDeadline += m_nReportPoseInterval;
		std::this_thread::sleep_until( pollDeadline );
	}
	LOG(INFO) << "ReportPoseThread:exit!" ;
}
void CHandControllerDevice::GetButtonState(const KeyBoardForControllerButton& button_state) {
	VRControllerState_t new_state = { 0 };
	new_state.unPacketNum = m_ControllerState.unPacketNum + 1;

	if (button_state.ButtonState & CONTROLLER_BUTTON_MENU)
		new_state.ulButtonPressed |= ButtonMaskFromId(k_EButton_ApplicationMenu);
	if (button_state.ButtonState & CONTROLLER_BUTTON_PAD_LEFT)
		new_state.ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Left);
	if (button_state.ButtonState & CONTROLLER_BUTTON_PAD_UP)
		new_state.ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Up);
	if (button_state.ButtonState & CONTROLLER_BUTTON_PAD_RIGHT)
		new_state.ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Right);
	if (button_state.ButtonState & CONTROLLER_BUTTON_PAD_DOWN)
		new_state.ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Down);
	if (button_state.ButtonState & CONTROLLER_BUTTON_SYSTEM)
		new_state.ulButtonPressed |= ButtonMaskFromId(k_EButton_System);
	if (button_state.ButtonState & CONTROLLER_BUTTON_TRIGGER)
		new_state.ulButtonPressed |= ButtonMaskFromId(k_EButton_SteamVR_Trigger);
	if (button_state.ButtonState & CONTROLLER_BUTTON_GRIP)
		new_state.ulButtonPressed |= ButtonMaskFromId(k_EButton_Grip);
//	if (button_state.ButtonState & CONTROLLER_BUTTON_TRACKEPAD_PRESS)
//		new_state.ulButtonPressed |= ;
	
	new_state.ulButtonTouched |= new_state.ulButtonPressed;

	uint64_t ulChangedTouched = new_state.ulButtonTouched ^ m_ControllerState.ulButtonTouched;
	uint64_t ulChangedPressed = new_state.ulButtonPressed ^ m_ControllerState.ulButtonPressed;

	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonTouched, ulChangedTouched & new_state.ulButtonTouched);
	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonPressed, ulChangedPressed & new_state.ulButtonPressed);
	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonUnpressed, ulChangedPressed & ~new_state.ulButtonPressed);
	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonUntouched, ulChangedTouched & ~new_state.ulButtonTouched);

	new_state.rAxis[1].x = button_state.rAxis[1].x;
	new_state.rAxis[1].y = 0.0f;

	if (new_state.rAxis[1].x != m_ControllerState.rAxis[1].x)
		vr::VRServerDriverHost()->TrackedDeviceAxisUpdated(m_nUniqueObjectId, 1, new_state.rAxis[1]);

	m_ControllerState = new_state;
}

void CHandControllerDevice::SendButtonUpdates(ButtonUpdate ButtonEvent, uint64_t ulMask)
{
	if (!ulMask)
		return;

	for (int i = 0; i< vr::k_EButton_Max; i++)
	{
		vr::EVRButtonId button = (vr::EVRButtonId)i;

		uint64_t bit = ButtonMaskFromId(button);

		if (bit & ulMask)
		{
			(vr::VRServerDriverHost()->*ButtonEvent)(m_nUniqueObjectId, button, 0.0);
		}
	}
}
#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
void CHandControllerDevice::SetXimmerseCobraHandle(const int& cobra_handle){
	m_nXimmerseXCobraHandle = cobra_handle;
	LOG(INFO) << "SetXimmerseDeviceHandle[" << m_cControllerRole << "]:" << cobra_handle;
}

void CHandControllerDevice::ReportXimmerseButton(const ximmerse::ControllerState& controller_state){
	float trigger_value = 0.0f,touch_pad_x = 0.0f,touch_pad_y = 0.0f;
	vr::EVRButtonId touch_pad_id = vr::k_EButton_Max;
	vr::VRControllerState_t new_state = { 0 };
	new_state.unPacketNum = m_ControllerState.unPacketNum + 1;

	trigger_value = controller_state.axes[ximmerse::CONTROLLER_AXIS_LEFT_TRIGGER];
	touch_pad_x = controller_state.axes[ximmerse::CONTROLLER_AXIS_LEFT_THUMB_X];
	touch_pad_y = controller_state.axes[ximmerse::CONTROLLER_AXIS_LEFT_THUMB_Y];

	if (controller_state.buttons & ximmerse::CONTROLLER_BUTTON_START){	
		m_uButton2PressCurrentTime = GetTickCount();
		if(m_uButton2PressCurrentTime - m_uButton2PressLastTime < DOUBLE_CLICK_INTERVAL 
			&& ((m_ControllerState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_System)) == 0)){
			//double click:recenter controller & hmd.
			LOG(INFO) << "double click system current_time=" << m_uButton2PressCurrentTime << ",last_time=" << m_uButton2PressLastTime << ",interval=" << m_uButton2PressCurrentTime - m_uButton2PressLastTime;
			if(m_uServerProviderHandleTrackedDevicePostMessageThreadID != -1){
				if (!PostThreadMessage(m_uServerProviderHandleTrackedDevicePostMessageThreadID, POST_MESSAGE_RECENTER, (WPARAM)&m_cControllerRole, 0)){
					LOG(WARNING) << "Controller[" << m_cControllerRole  <<"] post message error=" << GetLastError();
				}
			}				
		}
		//LOG_EVERY_N(INFO,1) << "CONTROLLER_BUTTON_BACK!current_time=" <<m_uButtonPressCurrentTime << ",last_time=" << m_uButtonPressLastTime << ",m_ControllerState.ulButtonPressed=" << m_ControllerState.ulButtonPressed << "," << ((m_ControllerState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Dashboard_Back)) == 0);		
		m_uButton2PressLastTime = m_uButton2PressCurrentTime;

		new_state.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_System);
		LOG_EVERY_N(INFO,10) << "CONTROLLER_BUTTON_BACK!";
	}
	if (controller_state.buttons & ximmerse::CONTROLLER_BUTTON_BACK){
		m_uButtonPressCurrentTime = GetTickCount();
		if(m_uButtonPressCurrentTime - m_uButtonPressLastTime < DOUBLE_CLICK_INTERVAL 
			&& ((m_ControllerState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Dashboard_Back)) == 0)){
			//double click:recenter controller & hmd.
			LOG(INFO) << "double click back current_time=" << m_uButtonPressCurrentTime << ",last_time=" << m_uButtonPressLastTime << ",interval=" << m_uButtonPressCurrentTime - m_uButtonPressLastTime;
			if(m_uServerProviderHandleTrackedDevicePostMessageThreadID != -1){
				if (!PostThreadMessage(m_uServerProviderHandleTrackedDevicePostMessageThreadID, POST_MESSAGE_TURN_AROUND, (WPARAM)&m_cControllerRole, 0)){
					LOG(WARNING) << "Controller[" << m_cControllerRole  <<"] post message error=" << GetLastError();
				}
			}				
		}
		new_state.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_Dashboard_Back);
		//LOG_EVERY_N(INFO,1) << "CONTROLLER_BUTTON_BACK!current_time=" <<m_uButtonPressCurrentTime << ",last_time=" << m_uButtonPressLastTime << ",m_ControllerState.ulButtonPressed=" << m_ControllerState.ulButtonPressed << "," << ((m_ControllerState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Dashboard_Back)) == 0);		
		m_uButtonPressLastTime = m_uButtonPressCurrentTime;
	}

	if(abs(touch_pad_x) > EPSILON || abs(touch_pad_y) > EPSILON){
		touch_pad_id = GetDPadButton(touch_pad_x,touch_pad_y);
		if(touch_pad_id != vr::k_EButton_Max){
			new_state.ulButtonPressed |= vr::ButtonMaskFromId(touch_pad_id);
		}
	}

	if (trigger_value > 0){
		new_state.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger);
		if(controller_state.buttons & ximmerse::CONTROLLER_BUTTON_BACK){
			new_state.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_Grip);
		}
	}
	else{
		new_state.ulButtonPressed &= ~vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger);	
	}

	
	if(controller_state.buttons & ximmerse::CONTROLLER_BUTTON_LEFT_THUMB 
		&& touch_pad_id != vr::k_EButton_Max){
		new_state.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad);
	}else{
		new_state.ulButtonPressed &= ~vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad);	
	}
	
	new_state.ulButtonTouched |= new_state.ulButtonPressed;

	uint64_t ulChangedTouched = new_state.ulButtonTouched ^ m_ControllerState.ulButtonTouched;
	uint64_t ulChangedPressed = new_state.ulButtonPressed ^ m_ControllerState.ulButtonPressed;

	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonTouched, ulChangedTouched & new_state.ulButtonTouched);
	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonPressed, ulChangedPressed & new_state.ulButtonPressed);
	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonUnpressed, ulChangedPressed & ~new_state.ulButtonPressed);
	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonUntouched, ulChangedTouched & ~new_state.ulButtonTouched);

	new_state.rAxis[1].x = trigger_value;
	new_state.rAxis[1].y = 0.0f;
	new_state.rAxis[0].x = touch_pad_x;
	new_state.rAxis[0].y = touch_pad_y;

	if(abs(new_state.rAxis[1].x - m_ControllerState.rAxis[1].x) > EPSILON)
		vr::VRServerDriverHost()->TrackedDeviceAxisUpdated(m_nUniqueObjectId, 1, new_state.rAxis[1]);
	if (abs(new_state.rAxis[0].x - m_ControllerState.rAxis[0].x) > EPSILON
		|| abs(new_state.rAxis[0].y - m_ControllerState.rAxis[0].y) > EPSILON)
		vr::VRServerDriverHost()->TrackedDeviceAxisUpdated(m_nUniqueObjectId, 0, new_state.rAxis[0]);

	m_ControllerState = new_state;
}

#endif
void CHandControllerDevice::SetSixDofModuleType(const ESixDofTrackingModule& six_dof_module){
	m_eSixModuleType = six_dof_module;
	switch(m_eSixModuleType){
		case NONE_SIX_DOF_TRACKING_MODULE:
			SetReportPoseInterval(NONE_SIX_DOF_CONTROLLER_POSE_REPORT_INTERVAL);
			break;
		case NOLO_SIX_DOF_TRACKING_MODULE:
			SetReportPoseInterval(NOLO_SIX_DOF_CONTROLLER_POSE_REPORT_INTERVAL);
			break;
		case XIMMERSE_SIX_DOF_TRACKING_MODULE:
			SetReportPoseInterval(XIMMERSE_SIX_DOF_CONTROLLER_POSE_REPORT_INTERVAL);
			break;
		default:
			LOG(WARNING) << "SetSixDofModuleType:unknown six module type = " << six_dof_module ;
			break;

	}
	LOG(INFO) << "set controller[" << m_cControllerRole <<  "] six dof module =" << six_dof_module;
}

void CHandControllerDevice::SetReportPoseInterval(const int& new_interval){
	m_nReportPoseInterval = std::chrono::milliseconds( new_interval );
	LOG(INFO) << "SetReportPoseInterval[" << m_cControllerRole << "]=" << new_interval << "ms.";
}

void CHandControllerDevice::SetPoseState(const bool& new_state){
	m_Pose.deviceIsConnected = new_state;
	m_Pose.poseIsValid = new_state;
	LOG(INFO) << "set controller pose state to " << new_state ? "connect" : "disconnect";
}

void CHandControllerDevice::SetSixDofData(void *six_dof_data){
#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
	if(m_eSixModuleType == XIMMERSE_SIX_DOF_TRACKING_MODULE){
		ximmerse::ControllerState *state = reinterpret_cast<ximmerse::ControllerState *>(six_dof_data);
        if (m_bTopCamera) {
            glm::vec4 vecPosition = glm::vec4((double)state->position.x, (double)-state->position.z, (double)state->position.y, 1.0f);
            glm::mat4 rts = glm::mat4(1.0);
            rts = glm::translate(rts, glm::vec3(0, m_bCameraHeight, 0));
            vecPosition = rts * vecPosition;
            m_Pose.vecPosition[0] = vecPosition.x;
            m_Pose.vecPosition[1] = vecPosition.y;
            m_Pose.vecPosition[2] = vecPosition.z;
        } else {
            GLM_F2Q(m_Pose.qRotation, state->rotation);		

            glm::mat4x4 modelMatrix;
            simple_math::Matrix4x4_TRS(modelMatrix, 0.0f, 0.0f, -1.5f,
                15.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 1.0f
            );
            simple_math::Matrix4x4_MultiplyMV(state->position, modelMatrix, state->position);

            m_Pose.vecPosition[0] = state->position[0] + m_fHmdXPositionOffset;
            m_Pose.vecPosition[1] = state->position[1] + m_fHmdYPositionOffset;
            m_Pose.vecPosition[2] = state->position[2] + m_fHmdZPositionOffset;
        }

		ReportXimmerseButton(*state);
	}
#endif

#ifdef USE_NOLO_SIX_DOF_TRACKING_MODULE
		if(m_eSixModuleType == NOLO_SIX_DOF_TRACKING_MODULE){
			NOLO::Controller *controller = reinterpret_cast<NOLO::Controller *>(six_dof_data);
			m_Pose.vecPosition[0] =  (double)controller->ControllerPosition.x;
			m_Pose.vecPosition[1] =  (double)controller->ControllerPosition.y;
			m_Pose.vecPosition[2] =  -(double)controller->ControllerPosition.z;
            if (m_bTopCamera) {
                glm::vec4 vecPosition = glm::vec4((double)controller->ControllerPosition.x, (double)controller->ControllerPosition.z, (double)controller->ControllerPosition.y, 1.0f);
                glm::mat4 rts = glm::mat4(1.0);
                rts = glm::translate(rts, glm::vec3(0, m_bCameraHeight, 0));
                vecPosition = rts * vecPosition;
                m_Pose.vecPosition[0] = vecPosition.x;
                m_Pose.vecPosition[1] = vecPosition.y;
                m_Pose.vecPosition[2] = vecPosition.z;
            }
			m_Pose.qRotation.x = controller->ControllerRotation.x;
			m_Pose.qRotation.y = controller->ControllerRotation.y;
			m_Pose.qRotation.z = -controller->ControllerRotation.z;
			m_Pose.qRotation.w = -controller->ControllerRotation.w;
			/*
			//handle throw action.
			if (controller->Buttons & NOLO_BUTTON_TRIGGER_PRESS) {
	
				m_bNoloTriggerDown = true;
			}
			if (m_bNoloTriggerDown && (controller->Buttons & NOLO_BUTTON_TRIGGER_PRESS) == 0) {
	
				m_bNoloTriggerDown = false;
				m_bNoloThrowFlag = true;
	
			}
			if (m_bNoloThrowFlag) {
	
			
	
				m_Pose.vecVelocity[0] = controller->vecVelocity.x;
				m_Pose.vecVelocity[1] = controller->vecVelocity.y;
				m_Pose.vecVelocity[2] = controller->vecVelocity.z;
	
				m_Pose.vecAngularVelocity[0] = controller->vecAngularVelocity.x;
				m_Pose.vecAngularVelocity[1] = controller->vecAngularVelocity.y;
				m_Pose.vecAngularVelocity[2] = controller->vecAngularVelocity.z;
	
				m_nNoloThrowCount++;
				if (m_Pose.vecVelocity[0] > 0.0 && m_Pose.vecVelocity[1] > 0.0 && m_nNoloThrowCount > 1 ) {
					m_nNoloThrowCount = 0;
					m_bNoloThrowFlag = false;
				}
			}
			else
			{
				m_Pose.vecVelocity[0] = 0.0f;
				m_Pose.vecVelocity[1] = 0.0f;
				m_Pose.vecVelocity[2] = 0.0f;
				m_Pose.vecAngularVelocity[0] = 0.0f;
				m_Pose.vecAngularVelocity[1] = 0.0f;
				m_Pose.vecAngularVelocity[2] = 0.0f;
			}	*/
			//report controller buttons.
			vr::VRControllerState_t vr_controller_state = {};
			if(controller->Buttons & NOLO_BUTTON_DPAD_PRESS){
				vr_controller_state.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad);
				vr_controller_state.rAxis[0].x = controller->ControllerTouchAxis.x;
				vr_controller_state.rAxis[0].y = controller->ControllerTouchAxis.y;
			}
			if(controller->Buttons & NOLO_BUTTON_TRIGGER_PRESS){
				vr_controller_state.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger);
				vr_controller_state.rAxis[1].x = 1.0f;
				vr_controller_state.rAxis[1].y = 0.0f;		
			}
			if(controller->Buttons & NOLO_BUTTON_MENU){
				vr_controller_state.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu);
			}
			if(controller->Buttons & NOLO_BUTTON_SYSTEM){
				vr_controller_state.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_System);
			}
			if(controller->Buttons & NOLO_BUTTON_GRIP){
				vr_controller_state.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_Grip);
			}
			ReportControllerButton(vr_controller_state,NULL);
	
	
		}
#endif

	if(m_eSixModuleType != NONE_SIX_DOF_TRACKING_MODULE){
		if(m_bTrunAround){
			vr::HmdQuaternion_t quaternion_rotate = HmdQuaternion_Init( 0, 0, 1, 0 );
			vr::HmdQuaternion_t quaternion_origin = HmdQuaternion_Init( m_Pose.qRotation.w,m_Pose.qRotation.x,m_Pose.qRotation.y,m_Pose.qRotation.z);
			//turn around rotation
			m_Pose.qRotation = glm_adapter::QuaternionMultiplyQuaternion(quaternion_rotate,m_Pose.qRotation);
			//turn around position
			glm_adapter::PointAroundPointRotate(quaternion_rotate,m_Pose.vecPosition,m_dHmdPositionWhenTurnAround,m_Pose.vecPosition);	
			/*LOG_EVERY_N(INFO,5*500) << "controller[" << m_cControllerRole << "] trun around pose:origin_YPR(" << simple_math::GetYawDegree(quaternion_origin) << "," << simple_math::GetPitchDegree(quaternion_origin) << "," << simple_math::GetRollDegree(quaternion_origin)
							<< "),rotate_YPR("	<< simple_math::GetYawDegree(quaternion_rotate) << "," << simple_math::GetPitchDegree(quaternion_rotate) << "," << simple_math::GetRollDegree(quaternion_rotate)
							<< "),dest_YPR("  << simple_math::GetYawDegree(m_Pose.qRotation) << "," << simple_math::GetPitchDegree(m_Pose.qRotation) << "," << simple_math::GetRollDegree(m_Pose.qRotation);				*/
		}
	}
}

void CHandControllerDevice::SetServerProviderHandleTrackedDevicePostMessageThreadID(uint64_t thread_id){
	m_uServerProviderHandleTrackedDevicePostMessageThreadID = thread_id;
	//LOG(INFO) << "Controller[" << m_cControllerRole << "] set server provider handle thread id =" << m_uServerProviderHandleTrackedDevicePostMessageThreadID;
}


void CHandControllerDevice::SetTurnAroundState(const bool& new_state){
	m_bTrunAround = new_state;
	LOG(INFO) << "set turn around state:" << new_state;
}
void CHandControllerDevice::SetHmdPositionWhenTurnAround(const double hmd_position[3]){
	m_dHmdPositionWhenTurnAround[0] = hmd_position[0];
	m_dHmdPositionWhenTurnAround[1] = hmd_position[1];
	m_dHmdPositionWhenTurnAround[2] = hmd_position[2];
}

void CHandControllerDevice::RecenterController(){
#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE	
	//recenter controller
	if(m_nXimmerseXCobraHandle >= 0){
		XDeviceSendMessage(m_nXimmerseXCobraHandle, ximmerse::kMessage_RecenterSensor, 0, 0);
		LOG(INFO) << "Controller[" << m_cControllerRole <<"] recenter." ;		
	}else{
		LOG(WARNING) << "RecenterController:handle is null.";
	}
	
#endif	

}

vr::EVRButtonId CHandControllerDevice::GetDPadButton(const float& float_x,const float& float_y){
	if(float_x > 1.0 || float_x < -1.0
	 ||float_y > 1.0 || float_y < -1.0){
		LOG(WARNING) << "GetDPadButton[" << m_cControllerRole << "]: error postion(" << float_x << "," << float_y << ")";
		return vr::k_EButton_Max;
	 }

	int x = float_x * 10000.0f,y = float_y * 10000.0f;
	 //UP:-y<x<y
	 if(x > -y && x < y){
	 	LOG_EVERY_N(INFO,1 * 90) << "GetDPadButton[" << m_cControllerRole << "]:k_EButton_DPad_Up";
	 	return vr::k_EButton_DPad_Up;
	 }
	 //DOWN:y<x<-y
	 if(x > y && x < -y){
	 	LOG_EVERY_N(INFO,1 * 90) << "GetDPadButton[" << m_cControllerRole << "]:k_EButton_DPad_Down";
	 	return vr::k_EButton_DPad_Down;
	 }
	 //LEFT:x<y<-x
	 if(y > x && y < -x){
	 	LOG_EVERY_N(INFO,1 * 90) << "GetDPadButton[" << m_cControllerRole << "]:k_EButton_DPad_Left";
	 	return vr::k_EButton_DPad_Left;
	 }
	 //RIGHT:-x<y<x
	 if(y > -x && y < x){
	 	LOG_EVERY_N(INFO,1 * 90) << "GetDPadButton[" << m_cControllerRole << "]:k_EButton_DPad_Right";
	 	return vr::k_EButton_DPad_Right;
	 }

	 LOG_EVERY_N(INFO,1 * 90) << "GetDPadButton[" << m_cControllerRole << "]:unknown region(" << x << "," << y << "),float(" << float_x << "," << "" << float_y <<")";
	 return vr::k_EButton_Max;
}

void CHandControllerDevice::ReportControllerButton(const vr::VRControllerState_t& controller_state,void *p_vendor_state){
	vr::EVRButtonId touch_pad_id = vr::k_EButton_Max;
	vr::VRControllerState_t new_state = controller_state;
	new_state.unPacketNum = m_ControllerState.unPacketNum + 1;
	
	if(new_state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)){
		touch_pad_id = GetDPadButton(new_state.rAxis[0].x,new_state.rAxis[0].y);
		if(touch_pad_id != vr::k_EButton_Max){
			new_state.ulButtonPressed |= vr::ButtonMaskFromId(touch_pad_id);
		}
	}
	
	new_state.ulButtonTouched |= new_state.ulButtonPressed;

	uint64_t ulChangedTouched = new_state.ulButtonTouched ^ m_ControllerState.ulButtonTouched;
	uint64_t ulChangedPressed = new_state.ulButtonPressed ^ m_ControllerState.ulButtonPressed;

	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonTouched, ulChangedTouched & new_state.ulButtonTouched);
	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonPressed, ulChangedPressed & new_state.ulButtonPressed);
	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonUnpressed, ulChangedPressed & ~new_state.ulButtonPressed);
	SendButtonUpdates(&vr::IVRServerDriverHost::TrackedDeviceButtonUntouched, ulChangedTouched & ~new_state.ulButtonTouched);

	if(abs(new_state.rAxis[1].x - m_ControllerState.rAxis[1].x) > EPSILON)
		vr::VRServerDriverHost()->TrackedDeviceAxisUpdated(m_nUniqueObjectId, 1, new_state.rAxis[1]);
	if (abs(new_state.rAxis[0].x - m_ControllerState.rAxis[0].x) > EPSILON
		|| abs(new_state.rAxis[0].y - m_ControllerState.rAxis[0].y) > EPSILON)
		vr::VRServerDriverHost()->TrackedDeviceAxisUpdated(m_nUniqueObjectId, 0, new_state.rAxis[0]);

	m_ControllerState = new_state;
}

