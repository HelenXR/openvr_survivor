#ifndef OPENVR_SURVIVOR_SRC_HEAD_MOUNT_DISPLAY_DEVICE_H_
#define OPENVR_SURVIVOR_SRC_HEAD_MOUNT_DISPLAY_DEVICE_H_
#include "common.h"
#include <thread>
// keys for use with the settings API
static const char * const k_pch_Sample_Section = "driver_helenxr";
static const char * const k_pch_Sample_SerialNumber_String = "serialNumber";
static const char * const k_pch_Sample_ModelNumber_String = "modelNumber";
static const char * const k_pch_Sample_WindowX_Int32 = "windowX";
static const char * const k_pch_Sample_WindowY_Int32 = "windowY";
static const char * const k_pch_Sample_WindowWidth_Int32 = "windowWidth";
static const char * const k_pch_Sample_WindowHeight_Int32 = "windowHeight";
static const char * const k_pch_Sample_RenderWidth_Int32 = "renderWidth";
static const char * const k_pch_Sample_RenderHeight_Int32 = "renderHeight";
static const char * const k_pch_Sample_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
static const char * const k_pch_Sample_DisplayFrequency_Float = "displayFrequency";

class CHeadMountDisplayDevice : public vr::ITrackedDeviceServerDriver,public vr::IVRDisplayComponent{
public:
	CHeadMountDisplayDevice();
	~CHeadMountDisplayDevice();
	//implement ITrackedDeviceServerDriver
	virtual EVRInitError Activate( uint32_t unObjectId ) override;
	virtual void Deactivate() override;
	virtual void EnterStandby() override;
	virtual void *GetComponent( const char *pchComponentNameAndVersion ) override;
	virtual void DebugRequest( const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize ) override;
	virtual DriverPose_t GetPose() override;	
	
	//implement IVRDisplayComponent
	virtual void GetWindowBounds( int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) override;
	virtual bool IsDisplayOnDesktop( ) override;
	virtual bool IsDisplayRealDisplay( ) override;
	virtual void GetRecommendedRenderTargetSize( uint32_t *pnWidth, uint32_t *pnHeight ) override;
	virtual void GetEyeOutputViewport( EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) override;
	virtual void GetProjectionRaw( EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom ) override;
	virtual DistortionCoordinates_t ComputeDistortion( EVREye eEye, float fU, float fV ) override;
	
	//other functions
	std::string GetSerialNumber() ;
	void CHeadMountDisplayDevice::ReportPoseThread();
private:
	vr::TrackedDeviceIndex_t m_unObjectId;
	vr::PropertyContainerHandle_t m_ulPropertyContainer;

	std::string m_sSerialNumber;
	std::string m_sModelNumber;

	int32_t m_nWindowX;
	int32_t m_nWindowY;
	int32_t m_nWindowWidth;
	int32_t m_nWindowHeight;
	int32_t m_nRenderWidth;
	int32_t m_nRenderHeight;
	float m_flSecondsFromVsyncToPhotons;
	float m_flDisplayFrequency;
	float m_flIPD;	
	
	std::thread m_tReportPoseThread;
	bool m_bReportPoseThreadState;                //false:stop true:running.
};
#endif
