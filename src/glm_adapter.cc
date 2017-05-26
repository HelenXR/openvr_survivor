#include "glm_adapter.h"

using namespace vr;
namespace glm_adapter{
	//HmdQuaternion_t mutiply HmdQuaternion_t
	HmdQuaternion_t QuaternionMultiplyQuaternion(const HmdQuaternion_t quaternion_rotate,const HmdQuaternion_t quaternion_origin){
		HmdQuaternion_t quaternion_dest;
		glm::quat quat_rotate = glm::quat((float)quaternion_rotate.w, (float)quaternion_rotate.x, (float)quaternion_rotate.y, (float)quaternion_rotate.z);
		glm::quat quat_origin = glm::quat((float)quaternion_origin.w, (float)quaternion_origin.x, (float)quaternion_origin.y, (float)quaternion_origin.z);
		glm::quat quat_dest = quat_rotate * quat_origin;
		quaternion_dest.x = quat_dest.x;
		quaternion_dest.y = quat_dest.y;
		quaternion_dest.z = quat_dest.z;
		quaternion_dest.w = quat_dest.w;		
		return quaternion_dest;
	}
	//HmdQuaternion_t  mutiply double [3]
	void QuaternionMultiplyDouble3(const vr::HmdQuaternion_t quaternion_rotate,const double position_origin[3],double position_dest[3]){
		glm::quat quat_rotate = glm::quat((float)quaternion_rotate.w, (float)quaternion_rotate.x, (float)quaternion_rotate.y, (float)quaternion_rotate.z);
		glm::vec3 vec3_origin = glm::vec3(position_origin[0], position_origin[1], position_origin[2]);
		glm::vec3 vec3_dest = quat_rotate * vec3_origin;
		position_dest[0] = vec3_dest.x;
		position_dest[1] = vec3_dest.y;
		position_dest[2] = vec3_dest.z;	
	}
	//yaw range:-90~90 pitch range:-180~180 roll range:-180~180.
	vr::HmdQuaternion_t RotateQuaternionByYawPitchRoll(const vr::HmdQuaternion_t quaternion_origin,double yaw_degree,double pitch_degree,double roll_degree){
		HmdQuaternion_t quaternion_dest = {0};
#if 0
		//convert quat to euler angle in degree.
		auto euler = degrees(eulerAngles(glm::quat((float)quaternion_origin.w,(float)quaternion_origin.x,(float)quaternion_origin.y,(float)quaternion_origin.z)));
		LOG(INFO) << "RotateQuaternionByYawPitchRoll:before euler(" << euler.x << "," << euler.y << "," << euler.z << ")";
		//add euler angle,pitch as x, yaw as y, roll as z.
		euler.x += (float)pitch_degree;
		euler.y += (float)yaw_degree;
		euler.z += (float)roll_degree;
		LOG(INFO) << "RotateQuaternionByYawPitchRoll:after euler(" << euler.x << "," << euler.y << "," << euler.z << ")";
		//convert euler angle in degree to quat.
		glm::quat quat = glm::quat{radians(euler)};
		quaternion_dest.x = quat.x;
		quaternion_dest.y = quat.y;
		quaternion_dest.z = quat.z;
		quaternion_dest.w = quat.w;
#else
		glm::quat quat_origin = glm::quat((float)quaternion_origin.w, (float)quaternion_origin.x, (float)quaternion_origin.y, (float)quaternion_origin.z);
		auto euler = degrees(eulerAngles(quat_origin));	
		glm::quat quat_dest;
		if(yaw_degree != 0)
			quat_dest = glm::rotate(quat_origin,glm::radians((float)yaw_degree),glm::vec3(0.f,1.f,0.f));
		else if (pitch_degree != 0)
			quat_dest = glm::rotate(quat_origin, glm::radians((float)pitch_degree), glm::vec3(1.f, 0.f, 0.f));
		else if (roll_degree != 0)
			quat_dest = glm::rotate(quat_origin, glm::radians((float)roll_degree), glm::vec3(0.f, 0.f, 1.f));
		euler = degrees(eulerAngles(quat_dest));
		quaternion_dest.x = quat_dest.x;
		quaternion_dest.y = quat_dest.y;
		quaternion_dest.z = quat_dest.z;
		quaternion_dest.w = quat_dest.w;
#endif	
		return quaternion_dest;
	}
	
}

