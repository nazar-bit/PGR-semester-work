#include "Params.hpp"
#include "SceneGraph.hpp"



namespace vasylnaz {

	const int WIN_WIDTH = 1024;
	const int WIN_HEIGHT = 768;
	const char* WIN_TITLE = "PGR SEMESTER PROJECT";
	const int CURVE_PRECISION = 500;

	bool HIDE_DEBUG = true;
	bool LAUNCHED = false;

    const char* PARAMS_FILE = "Params/params.txt";

	SceneGraph* CURRENT_SCENE = nullptr;
    std::vector<std::unique_ptr<Curve>> CAMERA_CURVES;
    glm::vec3 CAMERA_SAVED_POS = glm::vec3(-1.0f, 1.0f, 4.0f);
    glm::vec3 CAMERA_SAVED_TARGET = glm::vec3(-1.0f, 0.0f, 0.0f);
    glm::vec3 CAMERA_SAVED_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 MENU_TEXT_COLOR = glm::vec3(0.5f, 0.0f, 0.0f);
    glm::vec3 MENU_ACTIVATED_TEXT_COLOR = glm::vec3(0.0f, 0.5f, 0.0f);

    glm::vec3 pointDebugColor = glm::vec3(0.6f, 0.0f, 0.6f);
    glm::vec3 curveDebugColor = glm::vec3(1.0f, 0.0f, 0.0f);

    bool FOG = false;
    float FOG_DENSITY = 0.2f;
    float FOG_INCREMENT = 1.01f;
    float FOG_DISOLUTION_POINT = 0.01f;
    float FOG_MAX_DENSITY = 0.2f;

    bool CAMERA_BOUNDS = true;




    void loadParams() {
        std::cout << "CONFIG LOADING ...\n";
        CAMERA_CURVES.clear();
        std::ifstream file(PARAMS_FILE);

        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << PARAMS_FILE << std::endl;
            return;
        }

        std::vector<glm::vec3> curveData;
        std::string line;
        ConfigState state = ConfigState::NONE;

        auto saveCurveData = [&]() {
            if (!curveData.empty()) {
                CAMERA_CURVES.push_back(std::make_unique<Curve>(curveData));
                curveData.clear();
            }
            };

        while (std::getline(file, line)) {
            // Skip empty lines
            if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
                continue;
            }

            // Check markers
            if (line.find("#camera_curve") != std::string::npos) {
                saveCurveData();
                state = ConfigState::CAMERA_CURVE;
                continue;
            }
            if (line.find("#menu_text_color") != std::string::npos) {
                saveCurveData();
                state = ConfigState::MENU_TEXT_COLOR;
                continue;
            }
            if (line.find("#menu_activated_text_color") != std::string::npos) {
                saveCurveData();
                state = ConfigState::MENU_ACTIVATED_TEXT_COLOR;
                continue;
            }
            if (line.find("#point_debug_color") != std::string::npos) {
                saveCurveData();
                state = ConfigState::POINT_DEBUG_COLOR;
                continue;
            }
            if (line.find("#curve_debug_color") != std::string::npos) {
                saveCurveData();
                state = ConfigState::CURVE_DEBUG_COLOR;
                continue;
            }

            // If not hit - skip the line
            if (state == ConfigState::NONE) {
                continue;
            }

            // Vector values
            std::istringstream iss(line);
            float x, y, z;

            if (iss >> x >> y >> z) {
                glm::vec3 parsedVector(x, y, z);

                // Apply the parsed vector based on current state
                switch (state) {
                case ConfigState::CAMERA_CURVE:
                    curveData.push_back(parsedVector);
                    break;
                case ConfigState::MENU_TEXT_COLOR:
                    MENU_TEXT_COLOR = parsedVector;
                    break;
                case ConfigState::MENU_ACTIVATED_TEXT_COLOR:
                    MENU_ACTIVATED_TEXT_COLOR = parsedVector;
                    break;
                case ConfigState::POINT_DEBUG_COLOR:
                    pointDebugColor = parsedVector;
                    break;
                case ConfigState::CURVE_DEBUG_COLOR:
                    curveDebugColor = parsedVector;
                    break;
                default:
                    break;
                }
            }
            else {
                // malformed lines
            }
        }

        saveCurveData();
        file.close();
        std::cout << "CONFIG LOADING COMPLETE\n";
    }
}

