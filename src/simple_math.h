/** @file 
	@brief Some commonly used mathematical calculations function.

	@date 2017.07.14
	@author helenxr(helenhololens@gmail.com)
	
	if you want,you can use this in everywhere.
	if you want to contribute to this project,join us.
	note: The subsequent needs to be encapsulated into classes, using 	polymorphism will 
	be more reasonable, and there is time to make adjustments.
*/
#ifndef OPENVR_SURVIVOR_SRC_SIMPLE_MATH_H_
#define OPENVR_SURVIVOR_SRC_SIMPLE_MATH_H_
#include <openvr_driver.h>


#define M_PI    3.14159265358979323846264338327950288
#define EPSILON 0.000001f
/**
	Some commonly used mathematical calculations function.
*/
namespace simple_math{
	/**
		transform quaternion to euler in radian.
	*/
	vr::HmdVector3d_t QuaternionToEulerRadian(const vr::HmdQuaternion_t quaternion);
	/**
		tranform radian euler to quaternion.
	*/
	vr::HmdQuaternion_t RadianEulerToQuaternion(const vr::HmdVector3d_t euler);
	/**
		transform quaternion to euler in degree.
	*/
	vr::HmdVector3d_t QuaternionToEulerDegree(const vr::HmdQuaternion_t quaternion);
	/**
		tranform degree euler to quaternion.
	*/
	vr::HmdQuaternion_t DegreeEulerToQuaternion(const vr::HmdVector3d_t euler);
	/**
		tranform from degree to readian
	*/
	void DegreeToRadian(const double in[3], double out[3]);
	/**
		tranform from readian to degree
	*/
	void RadianToDegree(const double in[3], double out[3]);
	/**
		get yaw from quaternion in degree.
	*/
	double GetYawDegree(const vr::HmdQuaternion_t quaternion);
	/**
		get pitch from quaternion in degree.
	*/
	double GetPitchDegree(const vr::HmdQuaternion_t quaternion);
	/**
		get roll from quaternion in degree.
	*/
	double GetRollDegree(const vr::HmdQuaternion_t quaternion);

}
#endif

