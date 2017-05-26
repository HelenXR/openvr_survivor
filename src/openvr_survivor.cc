/** @file 
	@brief Every driver dynamic library needs to implement the standard driver factory function and use it to return implementations of the OpenVR driver interfaces.

	@date 2017.05.25
	@author helenxr(helenhololens@gmail.com)
	
	if you want,you can use this in everywhere.
	if you want to contribute to this project,join us.
*/

#include "common.h"
#include "server_provider.h"
#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )

CServerProvider g_CServerProvider;
HMD_DLL_EXPORT void *HmdDriverFactory(const char *pInterfaceName, int *pReturnCode)
{
	if (0 == strcmp(IServerTrackedDeviceProvider_Version, pInterfaceName))
	{
		//return &g_serverDriverNull;
		return (void *)&g_CServerProvider;
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
