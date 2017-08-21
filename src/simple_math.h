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
#include <ext.hpp>

#define kDeg2Rad 0.01745329f
#define kPi 3.141593f
#define kPiOver2 1.570796f

#define vec3_x 0
#define vec3_y 1
#define vec3_z 2

#define quat_x 0
#define quat_y 1
#define quat_z 2
#define quat_w 3

#define SIZE_VECTOR_3 12
#define SIZE_VECTOR_4 16
#define unzip_vec3(v) v.x, v.y, v.z
#define GLM_F2Q(Q,F) \
Q.x=F[0]; \
Q.y=F[1]; \
Q.z=F[2]; \
Q.w=F[3];

#define f2v(v,f) \
v.x=f[vec3_x];\
v.y=f[vec3_y];\
v.z=f[vec3_z];

#define v2f(f,v) \
f[vec3_x]=v.x;\
f[vec3_y]=v.y;\
f[vec3_z]=v.z;

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
	/**
		matrix4x4_TRS.
	*/
	void Matrix4x4_TRS(glm::mat4x4& dest,
		float tx,float ty,float tz,
		float rx,float ry,float rz,
		float sx,float sy,float sz
	);
	/**
		Matrix4x4_MultiplyMV	
	*/
	void Matrix4x4_MultiplyMV(float* dest,glm::mat4x4& m,float* src);

}
#endif

