#include "Light.hpp"


namespace vasylnaz {

	glm::vec3 GLOBAL_AMBIENT = glm::vec3(0.11f);
	long LightSource::global_light_id = 0;


    void LightBlock::addLight(const LightSource& LS) {
        if (LBD.numLights >= MAX_LIGHTS) return;

        scene_light.emplace_back(LS);
        LBD.lights[LBD.numLights++] = LS.getLight();
    }


    void LightBlock::updateViewSpacePositions(const glm::mat4& viewMatrix) {
        for (int i = 0; i < LBD.numLights; ++i) {
            Light currentLight = scene_light[i].getLight();
            currentLight.position = viewMatrix * currentLight.position;
            if (currentLight.spotlight != glm::vec4(0.0f))
            {
                currentLight.spotlight = glm::normalize(viewMatrix * currentLight.spotlight);
            }
            LBD.lights[i] = currentLight;
        }
    }
}