#ifndef OPENVR_SURVIVOR_SRC_GLM_ADAPTER_H_
#define OPENVR_SURVIVOR_SRC_GLM_ADAPTER_H_
#include <gtc/quaternion.hpp>
#include <openvr_driver.h>


namespace glm_adapter{
	//HmdQuaternion_t mutiply HmdQuaternion_t
	vr::HmdQuaternion_t QuaternionMultiplyQuaternion(const vr::HmdQuaternion_t quaternion_rotate,const vr::HmdQuaternion_t quaternion_origin);
	//HmdQuaternion_t  mutiply double [3]
	void QuaternionMultiplyDouble3(const vr::HmdQuaternion_t quaternion_rotate,const double position_origin[3],double position_dest[3]);
}
#endif