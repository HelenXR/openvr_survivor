#include "server_provider.h"

CServerProvider::CServerProvider(){

}

CServerProvider::~CServerProvider(){

}

vr::EVRInitError CServerProvider::Init( IVRDriverContext *pDriverContext ){
	LOG(INFO) << "CServerProvider:init!" ;
	//init modules contains:VRServerDriverHost,VRSettings,VRProperties,VRDriverLog.
	VR_INIT_SERVER_DRIVER_CONTEXT( pDriverContext );
	
	return VRInitError_None;
}

void CServerProvider::Cleanup(){
	google::ShutdownGoogleLogging();
}

const char * const *CServerProvider::GetInterfaceVersions(){
	return k_InterfaceVersions;
}

void CServerProvider::RunFrame(){

}

bool CServerProvider::ShouldBlockStandbyMode(){
	return false;
}

void CServerProvider::EnterStandby(){

}

void CServerProvider::LeaveStandby(){

}
