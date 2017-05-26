/** @file 
	@brief adapter to glm,convenient openVR driver to use glm library.

	@date 2017.05.25
	@author helenxr(helenhololens@gmail.com)
	
	if you want,you can use this in everywhere.
	if you want to contribute to this project,join us.
*/
#ifndef OPENVR_SURVIVOR_SRC_GLM_ADAPTER_H_
#define OPENVR_SURVIVOR_SRC_GLM_ADAPTER_H_
#include <gtc/quaternion.hpp>
#include <openvr_driver.h>

/**
	adapter to glm,convenient openVR driver to use glm library.
*/
namespace glm_adapter{
	/**
		HmdQuaternion_t * HmdQuaternion_t
		@param[in] quaternion_rotate rotate quaternion.
		@param[in] quaternion_origin origin quaternion.
		@return	dest quaternion.
	*/
	vr::HmdQuaternion_t QuaternionMultiplyQuaternion(const vr::HmdQuaternion_t quaternion_rotate,const vr::HmdQuaternion_t quaternion_origin);
	/**
		HmdQuaternion_t * double[3]
		@param[in] quaternion_rotate rotate quaternion.
		@param[in] position_origin origin double[3].
		@param[out] position_dest dest double[3].
	*/
	void QuaternionMultiplyDouble3(const vr::HmdQuaternion_t quaternion_rotate,const double position_origin[3],double position_dest[3]);
	/**
		Rotates a quaternion from a vector of 3 components axis and an angle.
		@param[in] quaternion_origin origin quaternion.
		@param[in] yaw_degree rotate in y axis some degree.
		@param[in] pitch_degree rotate in x axis some degree.
		@param[in] roll_degree rotate in z axis some degree.	
		@return  dest quaternion.
	*/
	vr::HmdQuaternion_t RotateQuaternionByYawPitchRoll(const vr::HmdQuaternion_t quaternion_origin,double yaw_degree,double pitch_degree,double roll_degree);	
}
#endif