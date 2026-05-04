#include "Params.hpp"
#include "SceneGraph.hpp"



namespace vasylnaz {

	const int WIN_WIDTH = 1024;
	const int WIN_HEIGHT = 768;
	const char* WIN_TITLE = "PGR SEMESTER PROJECT";
	const int CURVE_PRECISION = 500;

	bool HIDE_DEBUG = false;
	bool LAUNCHED = false;

    const char* PARAMS_FILE = "Params/params.txt";

	SceneGraph* CURRENT_SCENE = nullptr;
    std::vector<std::unique_ptr<Curve>> CAMERA_CURVES;
    glm::vec3 CAMERA_SAVED_POS = glm::vec3(-1.0f, 1.0f, 4.0f);
    glm::vec3 CAMERA_SAVED_TARGET = glm::vec3(-1.0f, 0.0f, 0.0f);
    glm::vec3 CAMERA_SAVED_UP = glm::vec3(0.0f, 1.0f, 0.0f);




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
        bool pastMarker = false;

        while (std::getline(file, line)) {
            if (line.find("#camera_curve") != std::string::npos) {
                if (!curveData.empty()) {
                    CAMERA_CURVES.push_back(std::make_unique<Curve>(curveData));
                    curveData.clear();
                }
                pastMarker = true;
                continue;
            }

            if (!pastMarker) {
                continue;
            }

            if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
                continue;
            }

            std::istringstream iss(line);
            float x, y, z;

            if (iss >> x >> y >> z) {
                curveData.push_back(glm::vec3(x, y, z));
            }
            else {
                // Future
            }
        }

        if (!curveData.empty()) {
            CAMERA_CURVES.push_back(std::make_unique<Curve>(curveData));
        }

        file.close();
        std::cout << "CONFIG LOADING COMPLETE\n";
    }
}

