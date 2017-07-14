#include "server_provider.h"

CServerProvider::CServerProvider(){

}

CServerProvider::~CServerProvider(){

}

vr::EVRInitError CServerProvider::Init( IVRDriverContext *pDriverContext ){

	//set log dir:steam/logs,you can set,you can set the path you want, but you need to make sure 
	//that the folder exists.
	FLAGS_log_dir = "../../../logs";
    //programe name:"openvr_survivor_glog"
    google::InitGoogleLogging("openvr_survivor_glog");
    LOG(INFO) << "CServerProvider init.";
	
	//init modules contains:VRServerDriverHost,VRSettings,VRProperties,VRDriverLog.
	VR_INIT_SERVER_DRIVER_CONTEXT( pDriverContext );
	
	//create HMD device
	m_pHeadMountDisplay = new CHeadMountDisplayDevice();
	vr::VRServerDriverHost()->TrackedDeviceAdded( m_pHeadMountDisplay->GetSerialNumber().c_str(), vr::TrackedDeviceClass_HMD, m_pHeadMountDisplay );
	
	//create controller device.
	m_pHandControllerDevice[LEFT_HAND_CONTROLLER] = new CHandControllerDevice("Left Controller",TrackedControllerRole_LeftHand);
	vr::VRServerDriverHost()->TrackedDeviceAdded(m_pHandControllerDevice[0]->GetSerialNumber(), vr::TrackedDeviceClass_Controller, m_pHandControllerDevice[0]);
	m_pHandControllerDevice[RIGHT_HAND_CONTROLLER] = new CHandControllerDevice("Right Controller",TrackedControllerRole_RightHand);
	vr::VRServerDriverHost()->TrackedDeviceAdded(m_pHandControllerDevice[1]->GetSerialNumber(), vr::TrackedDeviceClass_Controller, m_pHandControllerDevice[1]);
    
	LOG(INFO) << "CServerProvider end.";
	return VRInitError_None;
}

void CServerProvider::Cleanup(){
	LOG(INFO) << "CServerProvider::Cleanup!";
	google::ShutdownGoogleLogging();
}

const char * const *CServerProvider::GetInterfaceVersions(){
	return k_InterfaceVersions;
}

void CServerProvider::RunFrame(){
	KeyBoardDebugFunction();
}

bool CServerProvider::ShouldBlockStandbyMode(){
	return false;
}

void CServerProvider::EnterStandby(){

}

void CServerProvider::LeaveStandby(){

}

//Circle 10ms
void CServerProvider::KeyBoardDebugFunction(){
	static int loop_cnt = 0;
	static bool key_board_debug = false;
	if(loop_cnt++ < 9)//period:10*10ms
		return;
	loop_cnt = 0;
	if(KBC(VK_RCONTROL) && (KBC(VK_LCONTROL))){
		key_board_debug = !key_board_debug;
		LOG(INFO) << "key_board_debug:" << key_board_debug; 
	}
	if(key_board_debug){
		if(KBC(VK_KEY_1)){
			m_pHeadMountDisplay->RecenterHMD();
		}	
		if(KBC(VK_KEY_2)){
			m_pHeadMountDisplay->SetForwardDirectionInYaw(simple_math::GetYawDegree(m_pHeadMountDisplay->GetMemberPose().qRotation));
		}	
	}

}

