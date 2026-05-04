#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>



namespace vasylnaz {

	glm::quat calculateRotation(glm::vec3 oldPos, glm::vec3 newPos);

	glm::quat calculateRotationCurve(glm::vec3 oldPos, glm::vec3 newPos);

	extern float GlobalTime;
	extern float PauseStart;
	extern float PauseAdjustment;

	extern float NAN_FL;

	extern glm::vec3 defaultBackward;
	extern glm::vec3 defaultUp;
	extern glm::vec3 defaultRight;

}