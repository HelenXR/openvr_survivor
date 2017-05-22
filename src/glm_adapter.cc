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
}

