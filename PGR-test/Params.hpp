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

	extern SceneGraph* current_scene;
	extern std::vector<std::unique_ptr<Curve>> camera_curves;


	void loadParams();
}



