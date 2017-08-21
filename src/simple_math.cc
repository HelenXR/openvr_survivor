#include "simple_math.h"

namespace simple_math{
	/**
		transform quaternion to euler in radian.
	*/
	vr::HmdVector3d_t QuaternionToEulerRadian(const vr::HmdQuaternion_t quaternion)
	{
		vr::HmdVector3d_t euler;
		double x,y,z,w;
		x = quaternion.x;
		y = quaternion.y;
		z = quaternion.z;
		w = quaternion.w;
		double test = x*y + z*w;
		
		if (test > 0.499) { // singularity at north pole
			euler.v[0] = 2 * atan2(x, w);
			euler.v[1] = M_PI / 2.0;
			euler.v[2] = 0;
			return euler;
		}
		if (test < -0.499) { // singularity at south pole
			euler.v[0] = -2 * atan2(x, w);
			euler.v[1] = -M_PI / 2;
			euler.v[2] = 0;
			return euler;
		}
		double sqx = x*x;
		double sqy = y*y;
		double sqz = z*z;
		euler.v[0] = atan2(2 * y*w - 2 * x*z, 1 - 2 * sqy - 2 * sqz);//YAW 
		euler.v[1] = asin(2 * test);//ROLL 
		euler.v[2] = atan2(2 * x*w - 2 * y*z, 1 - 2 * sqx - 2 * sqz);//PITCH 
		return euler;	
	}
	/**
		tranform radian euler to quaternion.
	*/
	vr::HmdQuaternion_t RadianEulerToQuaternion(const vr::HmdVector3d_t euler)
	{
		vr::HmdQuaternion_t quaternion;
		// Assuming the angles are in radians.
		double c1 = cos(euler.v[0] / 2);//yaw
		double s1 = sin(euler.v[0] / 2);
		double c2 = cos(euler.v[1] / 2);//pitch
		double s2 = sin(euler.v[1] / 2);
		double c3 = cos(euler.v[2] / 2);//roll
		double s3 = sin(euler.v[2] / 2);//
		double c1c2 = c1*c2;
		double s1s2 = s1*s2;
		
		quaternion.x = c1c2*s3 + s1s2*c3;
		quaternion.y = s1*c2*c3 + c1*s2*s3;
		quaternion.z = c1*s2*c3 - s1*c2*s3;
		quaternion.w = c1c2*c3 - s1s2*s3;
		return quaternion;
	}
	/**
		transform quaternion to euler in degree.
	*/
	vr::HmdVector3d_t QuaternionToEulerDegree(const vr::HmdQuaternion_t quaternion)
	{
		vr::HmdVector3d_t vector_radian = QuaternionToEulerRadian(quaternion),vector_degree;
		RadianToDegree(vector_radian.v,vector_degree.v);
		return vector_degree;	
	}
	/**
		tranform degree euler to quaternion.
	*/
	vr::HmdQuaternion_t DegreeEulerToQuaternion(const vr::HmdVector3d_t euler)
	{
		vr::HmdVector3d_t eule_radian;
		DegreeToRadian(euler.v, eule_radian.v);
		vr::HmdQuaternion_t quaternion = RadianEulerToQuaternion(eule_radian);
		return quaternion;
	}
	/**
		tranform from degree to readian
	*/
	void DegreeToRadian(const double in[3], double out[3])
	{
	    out[0] = in[0] * (M_PI / 180.0);
	    out[1] = in[1] * (M_PI / 180.0);
	    out[2] = in[2] * (M_PI / 180.0);
	}
	/**
		tranform from readian to degree
	*/
	void RadianToDegree(const double in[3], double out[3])
	{
	    out[0] = in[0] * (180.0 / M_PI);
	    out[1] = in[1] * (180.0 / M_PI);
	    out[2] = in[2] * (180.0 / M_PI);
	}
	/**
		get yaw from quaternion in degree.
	*/
	double GetYawDegree(const vr::HmdQuaternion_t quaternion){
		vr::HmdVector3d_t vector_degree = QuaternionToEulerDegree(quaternion);
		return vector_degree.v[0];
	}
	/**
		get pitch from quaternion in degree.
	*/
	double GetPitchDegree(const vr::HmdQuaternion_t quaternion){
		vr::HmdVector3d_t vector_degree = QuaternionToEulerDegree(quaternion);
		return vector_degree.v[2];
	}
	/**
		get roll from quaternion in degree.
	*/
	double GetRollDegree(const vr::HmdQuaternion_t quaternion){
		vr::HmdVector3d_t vector_degree = QuaternionToEulerDegree(quaternion);
		return vector_degree.v[1];
	}

	void Matrix4x4_TRS(glm::mat4x4& dest,
		float tx,float ty,float tz,
		float rx,float ry,float rz,
		float sx,float sy,float sz
	){
		// T
		glm::mat4x4 T=glm::translate(glm::vec3(tx, ty, tz));
		// R
		glm::mat4x4 R=glm::yawPitchRoll(ry*kDeg2Rad,rx*kDeg2Rad,rz*kDeg2Rad);
		// S
		glm::mat4x4 S=glm::scale(glm::vec3(sx, sy, sz));
		//
		dest=T*R*S;
	}
	
	void Matrix4x4_MultiplyMV(float* dest,glm::mat4x4& m,float* src){
		glm::vec4 v=m*glm::vec4(src[vec3_x],src[vec3_y],src[vec3_z],1.0f);
		v2f(dest,v);
	}


}

