#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>



namespace vasylnaz {

	/// @brief Calculates rotation between two vectors
	/// @param u First vector
	/// @param v Second vector
	/// @return Quaternion representing rotation between two vectors
	glm::quat calculateRotation(glm::vec3 u, glm::vec3 v);

	/// @brief Calculates rotation between two positions on the curve
	/// @param oldPos Old curve position
	/// @param newPos New curve position
	/// @return Quaternion representing rotation
	glm::quat calculateRotationCurve(glm::vec3 oldPos, glm::vec3 newPos);

	extern float GlobalTime;
	extern float PauseStart;
	extern float PauseAdjustment;

	extern float NAN_FL;

	extern glm::vec3 defaultBackward;
	extern glm::vec3 defaultUp;
	extern glm::vec3 defaultRight;
}