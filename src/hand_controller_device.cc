#include "hand_controller_device.h"

CHandControllerDevice::CHandControllerDevice(string serial_number,ETrackedControllerRole controller_role){
	LOG(INFO) << "CHandControllerDevice:" << serial_number;
	m_sSerialNumber = serial_number;

	//init m_pose struct
	memset( &m_Pose, 0, sizeof( m_Pose ) );
	m_Pose.willDriftInYaw = true;
	m_Pose.shouldApplyHeadModel = false;
	m_Pose.deviceIsConnected = true;
	m_Pose.poseIsValid = true;
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
			break;
		case TrackedControllerRole_RightHand:
			m_Pose.vecPosition[0] = HAND_CONTROLLER_RELATIVE_HMD_POSITION_X;
			m_Pose.vecPosition[1] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_Y;
			m_Pose.vecPosition[2] = -HAND_CONTROLLER_RELATIVE_HMD_POSITION_Z;
			m_eHandController = RIGHT_HAND_CONTROLLER;
			break;
		default:
			LOG(ERROR) << "CHandControllerDevice:" << controller_role;
	}

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
	m_Pose.vecPosition[0] = m_pKeyBoardMonitor->GetControllerPose(m_eHandController).vecPosition[0];
	m_Pose.vecPosition[1] = m_pKeyBoardMonitor->GetControllerPose(m_eHandController).vecPosition[1];
	m_Pose.vecPosition[2] = m_pKeyBoardMonitor->GetControllerPose(m_eHandController).vecPosition[2];
#endif
	//before report,must adapte to HMD!
	return m_Pose;
}

VRControllerState_t CHandControllerDevice::GetControllerState( ){
	m_ControllerState.unPacketNum++;
	return m_ControllerState;
}
bool CHandControllerDevice::TriggerHapticPulse( uint32_t unAxisId, uint16_t usPulseDurationMicroseconds ){
	LOG(INFO) << __FUNCTION__;
	return false;
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
	LOG(INFO) << "ReportPoseButtonThread:run!" ;
	auto retryInterval = std::chrono::milliseconds( 16 );
	auto pollDeadline = std::chrono::steady_clock::now();	
	
	while(m_bReportPoseButtonThreadState){
		
		//update pose
		if ( m_nUniqueObjectId != vr::k_unTrackedDeviceIndexInvalid )
		{
			vr::VRServerDriverHost()->TrackedDevicePoseUpdated( m_nUniqueObjectId, GetPose(), sizeof( DriverPose_t ) );
		}

		//update button state
		
		LOG_EVERY_N(INFO,60*10) << "controller pose button loop!" ;
		pollDeadline += retryInterval;
		std::this_thread::sleep_until( pollDeadline );
	}
	LOG(INFO) << "ReportPoseThread:exit!" ;
}
