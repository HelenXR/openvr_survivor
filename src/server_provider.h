#ifndef OPENVR_SURVIVOR_SRC_SERVER_PROVIDER_H_
#define OPENVR_SURVIVOR_SRC_SERVER_PROVIDER_H_
#include "common.h"

using namespace vr;
class CServerProvider : public IServerTrackedDeviceProvider{
public:
	CServerProvider();
	~CServerProvider();
	virtual vr::EVRInitError Init( IVRDriverContext *pDriverContext ) override;
	virtual void Cleanup() override;
	virtual const char * const *GetInterfaceVersions() override;
	virtual void RunFrame() override;
	
	// ------------  Power State Functions ----------------------- //
	virtual bool ShouldBlockStandbyMode() override;
	virtual void EnterStandby() override;
	virtual void LeaveStandby() override;
};

#endif
