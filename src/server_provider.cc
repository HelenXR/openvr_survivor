#include "server_provider.h"

CServerProvider::CServerProvider(){
	m_eSixDofTrackingModule = NONE_SIX_DOF_TRACKING_MODULE;
	m_bControllerState[LEFT_HAND_CONTROLLER] = false;
	m_bControllerState[RIGHT_HAND_CONTROLLER] = false;
	m_pThis = this;
}

CServerProvider::~CServerProvider(){

}

CServerProvider *CServerProvider::m_pThis = NULL;
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

	SixDofTrackingModuleInit();
	m_tMultiLightWeightFunctionThread = std::thread(&CServerProvider::MultiLightWeightFunctionThread ,this);
	m_bMultiLightWeightFunctionThreadState = true;
	
	m_tHandleTrackedDevicePostMessageThread = std::thread(&CServerProvider::HandleTrackedDevicePostMessageThread ,this);
	m_bHandleTrackedDevicePostMessageThreadState = true;
	m_uHandleTrackedDevicePostMessageThreadID = -1;	
	LOG(INFO) << "CServerProvider end.";
	return VRInitError_None;
}

void CServerProvider::Cleanup(){
	//clear thread resource
	m_bMultiLightWeightFunctionThreadState = false;
	m_tMultiLightWeightFunctionThread.join();
	m_bHandleTrackedDevicePostMessageThreadState = false;
	m_tHandleTrackedDevicePostMessageThread.join();	
	//clear six dof module
#ifdef USE_NOLO_SIX_DOF_TRACKING_MODULE
		NOLO::close_Nolo_Device();
#endif
#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
	XDeviceExit();	
#endif	

	LOG(INFO) << "CServerProvider::Cleanup!";
	FlushLogFiles(GLOG_INFO);
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

void CServerProvider::SixDofTrackingModuleInit(){
#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
			XDeviceGetContext(true);
			//XDeviceSetLogger(&log_delegate);
			XDeviceSetInt(ID_CONTEXT, ximmerse::kField_VIDInt, 0x1F3B);
			XDeviceSetInt(ID_CONTEXT, ximmerse::kField_PIDInt, 0x10f2);
			XDeviceSetInt(ID_CONTEXT, ximmerse::kField_CtxLogLevelInt, ximmerse::LOG_DEBUG);
			XDeviceSetInt(ID_CONTEXT, ximmerse::kField_CtxDeviceVersionInt, XIM_CV1);
		
			XDeviceInit();
			m_nXimmerseHandle[XIMMERSE_HAWK] = XDeviceGetInputDeviceHandle("XHawk-0");
			m_nXimmerseHandle[XIMMERSE_COBRA_0] = XDeviceGetInputDeviceHandle("XCobra-0");
			m_nXimmerseHandle[XIMMERSE_COBRA_1] = XDeviceGetInputDeviceHandle("XCobra-1");
			m_nXimmerseHandle[XIMMERSE_VRDEVICE] = XDeviceGetInputDeviceHandle("VRDevice");
	
			if((m_pHandControllerDevice[LEFT_HAND_CONTROLLER] != NULL) && (m_nXimmerseHandle[XIMMERSE_COBRA_0] >= 0)){
				m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetXimmerseCobraHandle(m_nXimmerseHandle[XIMMERSE_COBRA_0]);
			}
			if((m_pHandControllerDevice[RIGHT_HAND_CONTROLLER] != NULL) && (m_nXimmerseHandle[XIMMERSE_COBRA_1] >= 0)){
				m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetXimmerseCobraHandle(m_nXimmerseHandle[XIMMERSE_COBRA_1]);
			}
			LOG(INFO) << "SdkVersion:" << XDeviceGetInt(ID_CONTEXT, ximmerse::kField_CtxSdkVersionInt, 0) << ",DeviceVersion:"
				<< XDeviceGetInt(ID_CONTEXT, ximmerse::kField_CtxDeviceVersionInt, 0) << ",PlatformVersion:" << 
				XDeviceGetInt(ID_CONTEXT, ximmerse::kField_CtxPlatformVersionInt, 0);
			LOG(INFO) << "ximmerse handle(" << m_nXimmerseHandle[XIMMERSE_HAWK] << "," << m_nXimmerseHandle[XIMMERSE_COBRA_0] << 
					"," << m_nXimmerseHandle[XIMMERSE_COBRA_1] << "," << m_nXimmerseHandle[XIMMERSE_VRDEVICE] << ").";
					
			if(m_nXimmerseHandle[XIMMERSE_HAWK] < 0 || m_nXimmerseHandle[XIMMERSE_COBRA_0] < 0 || m_nXimmerseHandle[XIMMERSE_COBRA_1] < 0
				|| m_nXimmerseHandle[XIMMERSE_VRDEVICE] < 0){
				LOG(WARNING) << "ximmerse handle error(" << m_nXimmerseHandle[XIMMERSE_HAWK] << "," << m_nXimmerseHandle[XIMMERSE_COBRA_0] << 
					"," << m_nXimmerseHandle[XIMMERSE_COBRA_1] << "," << m_nXimmerseHandle[XIMMERSE_VRDEVICE] << ").";
			}
			if(m_nXimmerseHandle[XIMMERSE_HAWK] >= 0){
				Sleep(2*1000);//wait for ximmerse module work!.
				XDeviceUpdateInputState(m_nXimmerseHandle[XIMMERSE_HAWK]);
				int status = XDeviceGetInt(m_nXimmerseHandle[XIMMERSE_HAWK], ximmerse::kField_ConnectionStateInt, 0);
				if(status == ximmerse::kConnectionState_Connected){
					SetSixDofModuleType(XIMMERSE_SIX_DOF_TRACKING_MODULE);
				}else{
					XDeviceExit();	
					LOG(INFO) << "ximmerse hawk is not connected.hawk=" << m_nXimmerseHandle[XIMMERSE_HAWK] << ",state=" << status;
				}
				
			}
			else{
				LOG(INFO) << "ximmerse hawk handle error!";
			}
#endif

#ifdef USE_NOLO_SIX_DOF_TRACKING_MODULE
		//LOG(INFO) << "SixDofTrackingModuleInit:this=" << this;
		NOLO::registerConnectSuccessCallBack(NoloHMDModuleConnect,this);
		NOLO::registerDisConnectCallBack(NoloHMDModuleDisconnect,this);
		NOLO::registerNoloDataNotifyCallBack(NoloDataNotify,this);
		NOLO::registerExpandDataNotifyCallBack(NoloExpandDataNotify,this);
		Sleep(100);
		NOLO::search_Nolo_Device(); 
#endif

}

void CServerProvider::SetSixDofModuleType(ESixDofTrackingModule six_dof_module){
	m_eSixDofTrackingModule = six_dof_module;

	LOG(INFO) << "SetSixDofModuleType:" << six_dof_module;
	if(m_pHeadMountDisplay != NULL){
		m_pHeadMountDisplay->SetSixDofModuleType(m_eSixDofTrackingModule);
	}
	if(m_pHandControllerDevice[LEFT_HAND_CONTROLLER] != NULL){
		m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetSixDofModuleType(m_eSixDofTrackingModule);
	}
	if(m_pHandControllerDevice[RIGHT_HAND_CONTROLLER] != NULL){
		m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetSixDofModuleType(m_eSixDofTrackingModule);
	}	

}

void CServerProvider::MultiLightWeightFunctionThread(){
	Sleep(1*1000);
	LOG(INFO) << "MultiLightWeightFunctionThread:run." ;
	auto retryInterval = std::chrono::milliseconds( 10 );
	auto pollDeadline = std::chrono::steady_clock::now();
	int key_board_count = 0,update_six_dof_count = 0;
	
	while(m_bMultiLightWeightFunctionThreadState){
		if(key_board_count++ >= 10){//10*10ms for detect keyboard input.
			key_board_count = 0;
			KeyBoardDebugFunction();
		}
		if(update_six_dof_count++ >= 100){ //1 second for decect six dof module.
			update_six_dof_count = 0;
			UpdateSixDofModuleState();
			FlushLogFiles(GLOG_INFO);	   //1 second for flush glog log
		}
		UpdateSixDofModuleTrackingData();
		pollDeadline += retryInterval;
		std::this_thread::sleep_until( pollDeadline );
	}
	LOG(INFO) << "MultiLightWeightFunctionThread:exit." ;
}

void CServerProvider::UpdateSixDofModuleState(){
#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
	if(m_eSixDofTrackingModule == XIMMERSE_SIX_DOF_TRACKING_MODULE){
		//detect xhawk
		if(m_pHeadMountDisplay != NULL){
			XDeviceUpdateInputState(m_nXimmerseHandle[XIMMERSE_HAWK]);
			if(XDeviceGetInt(m_nXimmerseHandle[XIMMERSE_HAWK], ximmerse::kField_ConnectionStateInt, 0) != ximmerse::kConnectionState_Connected	){
				SetSixDofModuleType(NONE_SIX_DOF_TRACKING_MODULE);
				XDeviceExit();	
				LOG(WARNING) << "ximmerse six dof module is disconnect.";
			}
		}
		//detect controller state
		if(m_pHandControllerDevice[LEFT_HAND_CONTROLLER] != NULL){
			XDeviceUpdateInputState(m_nXimmerseHandle[XIMMERSE_COBRA_0]);
			if((XDeviceGetInt(m_nXimmerseHandle[XIMMERSE_COBRA_0], ximmerse::kField_ConnectionStateInt, 0) == ximmerse::kConnectionState_Connected )
				&& m_bControllerState[LEFT_HAND_CONTROLLER] == false){
				m_bControllerState[LEFT_HAND_CONTROLLER] = true;
				m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetPoseState(true);
			}
				
			if((XDeviceGetInt(m_nXimmerseHandle[XIMMERSE_COBRA_0], ximmerse::kField_ConnectionStateInt, 0) != ximmerse::kConnectionState_Connected )
				&& m_bControllerState[LEFT_HAND_CONTROLLER] == true){
				m_bControllerState[LEFT_HAND_CONTROLLER] = false;
				m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetPoseState(false);
			}
		}
		if(m_pHandControllerDevice[RIGHT_HAND_CONTROLLER] != NULL){
			XDeviceUpdateInputState(m_nXimmerseHandle[XIMMERSE_COBRA_1]);
			if((XDeviceGetInt(m_nXimmerseHandle[XIMMERSE_COBRA_1], ximmerse::kField_ConnectionStateInt, 0) == ximmerse::kConnectionState_Connected )
				&& m_bControllerState[RIGHT_HAND_CONTROLLER] == false){
				m_bControllerState[RIGHT_HAND_CONTROLLER] = true;
				m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetPoseState(true);
			}
				
			if((XDeviceGetInt(m_nXimmerseHandle[XIMMERSE_COBRA_1], ximmerse::kField_ConnectionStateInt, 0) != ximmerse::kConnectionState_Connected )
				&& m_bControllerState[RIGHT_HAND_CONTROLLER] == true){
				m_bControllerState[RIGHT_HAND_CONTROLLER] = false;
				m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetPoseState(false);
			}
		}			
	}
#endif
}

void CServerProvider::UpdateSixDofModuleTrackingData(){
#ifdef USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
	if(m_eSixDofTrackingModule == XIMMERSE_SIX_DOF_TRACKING_MODULE){
		//get hmd six dof data
		float position[3];
		XDeviceUpdateInputState(m_nXimmerseHandle[XIMMERSE_HAWK]);
		int error_code = XDeviceGetNodePosition(m_nXimmerseHandle[XIMMERSE_HAWK],0, 2, position);
		if (error_code & ximmerse::kTrackingResult_PositionTracked) {
			m_pHeadMountDisplay->SetSixDofData(position);
		}else{
			LOG(WARNING) << "UpdateSixDofModuleTrackingData:hmd pose is not tracking.";
		}
		//get left controller six dof data.
		if((m_pHandControllerDevice[LEFT_HAND_CONTROLLER] != NULL )
			&& (m_bControllerState[LEFT_HAND_CONTROLLER] == true)){
			ximmerse::ControllerState state;
			error_code = XDeviceGetInputState(m_nXimmerseHandle[XIMMERSE_COBRA_0], &state);
			if(error_code >= 0){
				m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetSixDofData(&state);
			}else{
				LOG(WARNING) << "UpdateSixDofModuleTrackingData:left controller pose is not tracking.";
			}
		}
		//get right contriller six dof data.
		if((m_pHandControllerDevice[RIGHT_HAND_CONTROLLER] != NULL )
			&& (m_bControllerState[RIGHT_HAND_CONTROLLER] == true)){
			ximmerse::ControllerState state;
			error_code = XDeviceGetInputState(m_nXimmerseHandle[XIMMERSE_COBRA_1], &state);
			if(error_code >= 0){				
				m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetSixDofData(&state);
			}else{
				LOG(WARNING) << "UpdateSixDofModuleTrackingData:right controller pose is not tracking.";
			}
		}
	}
#endif
}

void CServerProvider::HandleTrackedDevicePostMessageThread(){
	Sleep(1*1000);
	MSG msg;
	auto retryInterval = std::chrono::milliseconds( 100 );
	auto pollDeadline = std::chrono::steady_clock::now();
	std::stringstream ss;
	ss << std::this_thread::get_id();
	m_uHandleTrackedDevicePostMessageThreadID = std::stoull(ss.str());
	LOG(INFO) << "HandleTrackedDevicePostMessageThread(" << m_uHandleTrackedDevicePostMessageThreadID << "):run!" ;
	if(m_pHandControllerDevice[LEFT_HAND_CONTROLLER] != NULL){
		m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetServerProviderHandleTrackedDevicePostMessageThreadID(m_uHandleTrackedDevicePostMessageThreadID);
	}
	if(m_pHandControllerDevice[RIGHT_HAND_CONTROLLER] != NULL){
		m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetServerProviderHandleTrackedDevicePostMessageThreadID(m_uHandleTrackedDevicePostMessageThreadID);
	}
	bool turn_arounded = false;
	while(m_bHandleTrackedDevicePostMessageThreadState){
		if(PeekMessage(&msg, 0, POST_MESSAGE_START, POST_MESSAGE_END, PM_REMOVE)) //get msg from message queue
		{
			char controller_role = *((char *)msg.wParam);
			switch (msg.message)
			{
			case POST_MESSAGE_RECENTER://recenter hmd and controller
				LOG(INFO) <<"receive POST_MESSAGE_RECENTER wParam:"<< controller_role;
				if(turn_arounded){
					turn_arounded = false;
					m_pHeadMountDisplay->SetTurnAroundState(false);
					m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetTurnAroundState(false);
					m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetTurnAroundState(false);
					Sleep(300);
				}				
				if(controller_role == 'L'){
					m_pHeadMountDisplay->SetForwardDirectionInYaw(0.0f);
					m_pHeadMountDisplay->RecenterHMD();
					m_pHeadMountDisplay->SetTurnAroundState(false);		
					m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->RecenterController();
					m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetTurnAroundState(false);
				}else{
					m_pHeadMountDisplay->SetForwardDirectionInYaw(0.0f);
					m_pHeadMountDisplay->RecenterHMD();
					m_pHeadMountDisplay->SetTurnAroundState(false);		
					m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->RecenterController();
					m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetTurnAroundState(false);
				}
				//turn_arounded = false;			
				break;

			case POST_MESSAGE_TURN_AROUND:
				LOG(INFO) << "receive POST_MESSAGE_TURN_AROUND wParam:"<< controller_role;
				if(turn_arounded == false){
					turn_arounded = true;
					m_pHeadMountDisplay->SetTurnAroundState(true);
					m_pHeadMountDisplay->SetHmdPositionWhenTurnAround(m_pHeadMountDisplay->GetMemberPose().vecPosition);
					m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetTurnAroundState(true);
					m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetHmdPositionWhenTurnAround(m_pHeadMountDisplay->GetMemberPose().vecPosition);
					m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetTurnAroundState(true);
					m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetHmdPositionWhenTurnAround(m_pHeadMountDisplay->GetMemberPose().vecPosition);				
				}else{
					turn_arounded = false;
					m_pHeadMountDisplay->SetTurnAroundState(false);
					m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetTurnAroundState(false);
					m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetTurnAroundState(false);
				}	
			break;					
				break;

			default:
				LOG(WARNING) << "HandleTrackedDevicePostMessageThread:unknown type message=" << msg.message;
				break;
			}
		}		
		//LOG_EVERY_N(INFO,1 * 10) << "HandleTrackedDevicePostMessageThread:loop!" ;
		pollDeadline += retryInterval;
		std::this_thread::sleep_until( pollDeadline );
	}
	LOG(INFO) << "HandleTrackedDevicePostMessageThread:exit!" ;
}

#ifdef USE_NOLO_SIX_DOF_TRACKING_MODULE
void CServerProvider::NoloHMDModuleConnect(void *context){
	LOG(INFO) << "NoloHMDModuleConnect.m_pThis=" << m_pThis;
	m_pThis->SetSixDofModuleType(NOLO_SIX_DOF_TRACKING_MODULE);
}
void CServerProvider::NoloHMDModuleDisconnect(void *context){
	LOG(INFO) << "NoloHMDModuleDisconnect.";
	m_pThis->SetSixDofModuleType(NONE_SIX_DOF_TRACKING_MODULE);
}
void CServerProvider::NoloDataNotify(NOLO::NoloData noloData,void * context){
	//LOG_EVERY_N(INFO,5*130) << "NoloDataNotify.";
	VLOG_EVERY_N(1,5*130) << "NoloDataNotify:p(" << m_pThis->m_pHandControllerDevice[LEFT_HAND_CONTROLLER] << "," << m_pThis->m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]
	<< "),state(" << m_pThis->m_bControllerState[LEFT_HAND_CONTROLLER] << "," << m_pThis->m_bControllerState[RIGHT_HAND_CONTROLLER] << "),battery("
	<< noloData.left_Controller_Data.ControllerBattery << "," << noloData.right_Controller_Data.ControllerBattery << ")";

	if(m_pThis->m_pHeadMountDisplay != NULL){
		m_pThis->m_pHeadMountDisplay->SetSixDofData(&noloData.hmdData);
	}
	if(m_pThis->m_pHandControllerDevice[LEFT_HAND_CONTROLLER] != NULL){
		if((m_pThis->m_bControllerState[LEFT_HAND_CONTROLLER] == true) && 
			(noloData.left_Controller_Data.ControllerBattery == NOLO::ShutDown)){
			m_pThis->m_bControllerState[LEFT_HAND_CONTROLLER] = false;
			m_pThis->m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetPoseState(false);
		}
			
		if((m_pThis->m_bControllerState[LEFT_HAND_CONTROLLER] == false) && 
			(noloData.left_Controller_Data.ControllerBattery != NOLO::ShutDown)){
				m_pThis->m_bControllerState[LEFT_HAND_CONTROLLER] = true;
				m_pThis->m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetPoseState(true);
			}
			
		if(m_pThis->m_bControllerState[LEFT_HAND_CONTROLLER] == true){
			m_pThis->m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetSixDofData(&noloData.left_Controller_Data);	
		}
	}
	if(m_pThis->m_pHandControllerDevice[RIGHT_HAND_CONTROLLER] != NULL){
		if((m_pThis->m_bControllerState[RIGHT_HAND_CONTROLLER] == true) && 
			(noloData.right_Controller_Data.ControllerBattery == NOLO::ShutDown)){
			m_pThis->m_bControllerState[RIGHT_HAND_CONTROLLER] = false;
			m_pThis->m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetPoseState(false);
		}
			
		if((m_pThis->m_bControllerState[RIGHT_HAND_CONTROLLER] == false) && 
			(noloData.right_Controller_Data.ControllerBattery != NOLO::ShutDown)){
				m_pThis->m_bControllerState[RIGHT_HAND_CONTROLLER] = true;
				m_pThis->m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetPoseState(true);
		}
			
		if(m_pThis->m_bControllerState[RIGHT_HAND_CONTROLLER] == true){
			m_pThis->m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetSixDofData(&noloData.right_Controller_Data);	
		}
	}

}
void CServerProvider::NoloExpandDataNotify(NOLO::ExpandMsgType expandMsgType , void * context){
	static bool turn_arounded = false;
	LOG(INFO) << "NoloExpandDataNotify:" << expandMsgType;
	switch(expandMsgType){
		case NOLO::DoubleClickKeyTurnAround:
			if(turn_arounded == false){
				turn_arounded = true;
				m_pThis->m_pHeadMountDisplay->SetTurnAroundState(true);
				m_pThis->m_pHeadMountDisplay->SetHmdPositionWhenTurnAround(m_pThis->m_pHeadMountDisplay->GetMemberPose().vecPosition);
				m_pThis->m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetTurnAroundState(true);
				m_pThis->m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetHmdPositionWhenTurnAround(m_pThis->m_pHeadMountDisplay->GetMemberPose().vecPosition);
				m_pThis->m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetTurnAroundState(true);
				m_pThis->m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetHmdPositionWhenTurnAround(m_pThis->m_pHeadMountDisplay->GetMemberPose().vecPosition);				
			}else{
				turn_arounded = false;
				m_pThis->m_pHeadMountDisplay->SetTurnAroundState(false);
				m_pThis->m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetTurnAroundState(false);
				m_pThis->m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetTurnAroundState(false);
			}
			break;
		case NOLO::DoubleClickSystemRecenter:
			if(turn_arounded){
				turn_arounded = false;
				m_pThis->m_pHeadMountDisplay->SetTurnAroundState(false);
				m_pThis->m_pHandControllerDevice[LEFT_HAND_CONTROLLER]->SetTurnAroundState(false);
				m_pThis->m_pHandControllerDevice[RIGHT_HAND_CONTROLLER]->SetTurnAroundState(false);
				Sleep(300);
			}
			m_pThis->m_pHeadMountDisplay->SetForwardDirectionInYaw(0.0f);
			m_pThis->m_pHeadMountDisplay->RecenterHMD();
			//if turn_around = true  must be do this
			turn_arounded = false;			
			break;
		default:
			LOG(WARNING) << "NoloExpandDataNotify:unknown type(" << expandMsgType << ")!";
			break;
	}
}
#endif

