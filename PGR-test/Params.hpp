#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <glm/glm.hpp> 

#include "Curve.hpp"


namespace vasylnaz {

	class SceneGraph;

	extern const int WIN_WIDTH;
	extern const int WIN_HEIGHT;
	extern const char* WIN_TITLE;
	extern const int CURVE_PRECISION;

	extern bool HIDE_DEBUG;
	extern bool LAUNCHED;

	extern const char* PARAMS_FILE;

	extern SceneGraph* CURRENT_SCENE;
	extern std::vector<std::unique_ptr<Curve>> CAMERA_CURVES;
	const glm::vec3 CAMERA_DEFAULT_POS = glm::vec3(-1.0f, 1.0f, 4.0f);
	extern glm::vec3 CAMERA_SAVED_POS;
	const glm::vec3 CAMERA_DEFAULT_TARGET = glm::vec3(-1.0f, 0.0f, 0.0f);   // normalized vector
	extern glm::vec3 CAMERA_SAVED_TARGET;
	const glm::vec3 CAMERA_DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);   // normalized vector
	extern glm::vec3 CAMERA_SAVED_UP;

	extern glm::vec3 MENU_TEXT_COLOR;
	extern glm::vec3 MENU_ACTIVATED_TEXT_COLOR;

	extern glm::vec3 pointDebugColor;
	extern glm::vec3 curveDebugColor;

	extern bool FOG;
	extern float FOG_DENSITY;
	extern float FOG_INCREMENT;
	extern float FOG_DISOLUTION_POINT;
	extern float FOG_MAX_DENSITY;

	extern bool CAMERA_BOUNDS;


	enum class ConfigState {
		NONE,
		CAMERA_CURVE,
		MENU_TEXT_COLOR,
		MENU_ACTIVATED_TEXT_COLOR,
		POINT_DEBUG_COLOR,
		CURVE_DEBUG_COLOR
	};

	/// @brief Loads parameters from the params.txt
	void loadParams();
}



