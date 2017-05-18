#include "head_mount_display_device.h"

CHeadMountDisplayDevice::CHeadMountDisplayDevice(){
	char buf[1024];
	LOG(INFO) << "CHeadMountDisplayDevice Construction.";
	//get parameters from settings file.
	m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
	m_ulPropertyContainer = vr::k_ulInvalidPropertyContainer;
	
	m_flIPD = vr::VRSettings()->GetFloat( k_pch_SteamVR_Section, k_pch_SteamVR_IPD_Float );

	vr::VRSettings()->GetString( k_pch_Sample_Section, k_pch_Sample_SerialNumber_String, buf, sizeof( buf ) );
	m_sSerialNumber = buf;

	vr::VRSettings()->GetString( k_pch_Sample_Section, k_pch_Sample_ModelNumber_String, buf, sizeof( buf ) );
	m_sModelNumber = buf;

	m_nWindowX = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowX_Int32 );
	m_nWindowY = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowY_Int32 );
	m_nWindowWidth = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowWidth_Int32 );
	m_nWindowHeight = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowHeight_Int32 );
	m_nRenderWidth = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_RenderWidth_Int32 );
	m_nRenderHeight = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_RenderHeight_Int32 );
	m_flSecondsFromVsyncToPhotons = vr::VRSettings()->GetFloat( k_pch_Sample_Section, k_pch_Sample_SecondsFromVsyncToPhotons_Float );
	m_flDisplayFrequency = vr::VRSettings()->GetFloat( k_pch_Sample_Section, k_pch_Sample_DisplayFrequency_Float );

	LOG(INFO) << "CHeadMountDisplayDevice: Serial Number:" << m_sSerialNumber.c_str();
	LOG(INFO) << "CHeadMountDisplayDevice: Model Number:" << m_sModelNumber.c_str() ;
	LOG(INFO) << "CHeadMountDisplayDevice: Window:" << m_nWindowX << "," << m_nWindowY << "," << m_nWindowWidth << "," << m_nWindowHeight ;
	LOG(INFO) << "CHeadMountDisplayDevice: Render Target:" << m_nRenderWidth << "," << m_nRenderHeight;
	LOG(INFO) << "CHeadMountDisplayDevice: Seconds from Vsync to Photons:" << m_flSecondsFromVsyncToPhotons;
	LOG(INFO) << "CHeadMountDisplayDevice: Display Frequency:" << m_flDisplayFrequency;
	LOG(INFO) << "CHeadMountDisplayDevice: IPD:" << m_flIPD;
	
	
	//create report hmd pose thread
	m_tReportPoseThread = std::thread(&CHeadMountDisplayDevice::ReportPoseThread ,this);
	m_bReportPoseThreadState = true;
	
#if defined(HMD_ROTATE_BY_KEYBOARD) || defined(HMD_POSITION_BY_KEYBOARD) 
	m_pKeyBoardMonitor = KeyBoardMonitor::GetInstance();
#endif
}

CHeadMountDisplayDevice::~CHeadMountDisplayDevice(){
	delete m_pKeyBoardMonitor;
}

EVRInitError CHeadMountDisplayDevice::Activate(uint32_t unObjectId){
	m_unObjectId = unObjectId;
	m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer( m_unObjectId );


	vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, Prop_ModelNumber_String, m_sModelNumber.c_str() );
	vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, Prop_RenderModelName_String, m_sModelNumber.c_str() );
	vr::VRProperties()->SetFloatProperty( m_ulPropertyContainer, Prop_UserIpdMeters_Float, m_flIPD );
	vr::VRProperties()->SetFloatProperty( m_ulPropertyContainer, Prop_UserHeadToEyeDepthMeters_Float, 0.f );
	vr::VRProperties()->SetFloatProperty( m_ulPropertyContainer, Prop_DisplayFrequency_Float, m_flDisplayFrequency );
	vr::VRProperties()->SetFloatProperty( m_ulPropertyContainer, Prop_SecondsFromVsyncToPhotons_Float, m_flSecondsFromVsyncToPhotons );
	// return a constant that's not 0 (invalid) or 1 (reserved for Oculus)
	vr::VRProperties()->SetUint64Property( m_ulPropertyContainer, Prop_CurrentUniverseId_Uint64, 2 );

	// avoid "not fullscreen" warnings from vrmonitor
	vr::VRProperties()->SetBoolProperty( m_ulPropertyContainer, Prop_IsOnDesktop_Bool, false );

	// Icons can be configured in code or automatically configured by an external file "drivername\resources\driver.vrresources".
	// Icon properties NOT configured in code (post Activate) are then auto-configured by the optional presence of a driver's "drivername\resources\driver.vrresources".
	// In this manner a driver can configure their icons in a flexible data driven fashion by using an external file.
	//
	// The structure of the driver.vrresources file allows a driver to specialize their icons based on their HW.
	// Keys matching the value in "Prop_ModelNumber_String" are considered first, since the driver may have model specific icons.
	// An absence of a matching "Prop_ModelNumber_String" then considers the ETrackedDeviceClass ("HMD", "Controller", "GenericTracker", "TrackingReference")
	// since the driver may have specialized icons based on those device class names.
	//
	// An absence of either then falls back to the "system.vrresources" where generic device class icons are then supplied.
	//
	// Please refer to "bin\drivers\sample\resources\driver.vrresources" which contains this sample configuration.
	//
	// "Alias" is a reserved key and specifies chaining to another json block.
	//
	// In this sample configuration file (overly complex FOR EXAMPLE PURPOSES ONLY)....
	//
	// "Model-v2.0" chains through the alias to "Model-v1.0" which chains through the alias to "Model-v Defaults".
	//
	// Keys NOT found in "Model-v2.0" would then chase through the "Alias" to be resolved in "Model-v1.0" and either resolve their or continue through the alias.
	// Thus "Prop_NamedIconPathDeviceAlertLow_String" in each model's block represent a specialization specific for that "model".
	// Keys in "Model-v Defaults" are an example of mapping to the same states, and here all map to "Prop_NamedIconPathDeviceOff_String".
	//
	bool bSetupIconUsingExternalResourceFile = false;
	if ( !bSetupIconUsingExternalResourceFile )
	{
		// Setup properties directly in code.
		// Path values are of the form {drivername}\icons\some_icon_filename.png
		vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceOff_String, "{openvr_survivor}/icons/headset_sample_status_off.png" );
		vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceSearching_String, "{openvr_survivor}/icons/headset_sample_status_searching.gif" );
		vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceSearchingAlert_String, "{openvr_survivor}/icons/headset_sample_status_searching_alert.gif" );
		vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceReady_String, "{openvr_survivor}/icons/headset_sample_status_ready.png" );
		vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceReadyAlert_String, "{openvr_survivor}/icons/headset_sample_status_ready_alert.png" );
		vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceNotReady_String, "{openvr_survivor}/icons/headset_sample_status_error.png" );
		vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceStandby_String, "{openvr_survivor}/icons/headset_sample_status_standby.png" );
		vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceAlertLow_String, "{openvr_survivor}/icons/headset_sample_status_ready_low.png" );
	}

	return VRInitError_None;
}

void CHeadMountDisplayDevice::Deactivate(){
	LOG(INFO) << "CHeadMountDisplayDevice::Deactive: enter";
	m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
	m_bReportPoseThreadState = false;
}

void CHeadMountDisplayDevice::EnterStandby(){

}

void *CHeadMountDisplayDevice::GetComponent(const char *pchComponentNameAndVersion){
	if ( !_stricmp( pchComponentNameAndVersion, vr::IVRDisplayComponent_Version ) )
	{
		return (vr::IVRDisplayComponent*)this;
	}

	// override this to add a component to a driver
	return NULL;
}

void CHeadMountDisplayDevice::DebugRequest(const char *pchRequest,char *pchResponseBuffer,uint32_t unResponseBufferSize){

}

DriverPose_t CHeadMountDisplayDevice::GetPose(){
	DriverPose_t pose = { 0 };
	pose.poseIsValid = true;
	pose.result = TrackingResult_Running_OK;
	pose.deviceIsConnected = true;

	pose.qWorldFromDriverRotation = HmdQuaternion_Init( 1, 0, 0, 0 );
	pose.qDriverFromHeadRotation = HmdQuaternion_Init( 1, 0, 0, 0 );
	
#if defined(HMD_ROTATE_BY_KEYBOARD)
    pose.qRotation = m_pKeyBoardMonitor->GetHMDPose().qRotation;
#endif

#if defined(HMD_POSITION_BY_KEYBOARD)
	pose.vecPosition[0] = m_pKeyBoardMonitor->GetHMDPose().vecPosition[0];
	pose.vecPosition[1] = m_pKeyBoardMonitor->GetHMDPose().vecPosition[1];
	pose.vecPosition[2] = m_pKeyBoardMonitor->GetHMDPose().vecPosition[2];
#endif
	return pose;
}

void CHeadMountDisplayDevice::GetWindowBounds( int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) 
{
	*pnX = m_nWindowX;
	*pnY = m_nWindowY;
	*pnWidth = m_nWindowWidth;
	*pnHeight = m_nWindowHeight;
}

bool CHeadMountDisplayDevice::IsDisplayOnDesktop() 
{
	return true;
}
bool CHeadMountDisplayDevice::IsDisplayRealDisplay() 
{
	return true;
	//return false;
}

void CHeadMountDisplayDevice::GetRecommendedRenderTargetSize( uint32_t *pnWidth, uint32_t *pnHeight ) 
{
	*pnWidth = m_nRenderWidth;
	*pnHeight = m_nRenderHeight;
}

void CHeadMountDisplayDevice::GetEyeOutputViewport( EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) 
{
	*pnY = 0;
	*pnWidth = m_nWindowWidth / 2;
	*pnHeight = m_nWindowHeight;
	
	if ( eEye == Eye_Left )
	{
		*pnX = 0;
	}
	else
	{
		*pnX = m_nWindowWidth / 2;
	}
}

void CHeadMountDisplayDevice::GetProjectionRaw( EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom ) 
{
	*pfLeft = -1.0;
	*pfRight = 1.0;
	*pfTop = -1.0;
	*pfBottom = 1.0;	
}

DistortionCoordinates_t CHeadMountDisplayDevice::ComputeDistortion( EVREye eEye, float fU, float fV ) 
{
	DistortionCoordinates_t coordinates;
	coordinates.rfBlue[0] = fU;
	coordinates.rfBlue[1] = fV;
	coordinates.rfGreen[0] = fU;
	coordinates.rfGreen[1] = fV;
	coordinates.rfRed[0] = fU;
	coordinates.rfRed[1] = fV;
	return coordinates;
}

std::string CHeadMountDisplayDevice::GetSerialNumber(){
	return m_sSerialNumber;
}

void CHeadMountDisplayDevice::ReportPoseThread(){
	Sleep(1*1000);
	LOG(INFO) << "ReportPoseThread:run!" ;
	auto retryInterval = std::chrono::milliseconds( 16 );
	auto pollDeadline = std::chrono::steady_clock::now();	
	
	while(m_bReportPoseThreadState){
		if ( m_unObjectId != vr::k_unTrackedDeviceIndexInvalid )
		{
			vr::VRServerDriverHost()->TrackedDevicePoseUpdated( m_unObjectId, GetPose(), sizeof( DriverPose_t ) );
		}
		LOG_EVERY_N(INFO,60*10) << "pose loop!" ;
		
		FlushLogFiles(GLOG_INFO);
		pollDeadline += retryInterval;
		std::this_thread::sleep_until( pollDeadline );
	}
	LOG(INFO) << "ReportPoseThread:exit!" ;
}






















































