#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>



namespace vasylnaz {

	glm::quat calculateRotation(glm::vec3 oldPos, glm::vec3 newPos);

	extern float GlobalTime;

}