#include "common.h"

#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )

HMD_DLL_EXPORT void *HmdDriverFactory(const char *pInterfaceName, int *pReturnCode)
{
	//set log dir:steam/logs,you can set,you can set the path you want, but you need to make sure 
	//that the folder exists.
	FLAGS_log_dir = "../../../logs";
    //programe name:"openvr_survivor_glog"
    google::InitGoogleLogging("openvr_survivor_glog");
    LOG(INFO) << "google log first info level message!";
    //...... DoSomething
    //Shutdown google's logging library.
    google::ShutdownGoogleLogging();
	if (0 == strcmp(IServerTrackedDeviceProvider_Version, pInterfaceName))
	{
		//return &g_serverDriverNull;
		return NULL;
	}
	if (0 == strcmp(IVRWatchdogProvider_Version, pInterfaceName))
	{
		//return &g_watchdogDriverNull;
		return NULL;
	}

	if (pReturnCode)
		*pReturnCode = VRInitError_Init_InterfaceNotFound;

	return NULL;
}