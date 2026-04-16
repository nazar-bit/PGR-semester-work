#include "Light.hpp"


namespace vasylnaz {

	glm::vec3 GLOBAL_AMBIENT = glm::vec3(0.11f);
	long LightSource::global_light_id = 0;


    Spotlight DEFAULT_SPOT = Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f);
    Attenuation DEFAULT_ATTEN = Attenuation(1.0f, 0.09f, 0.032f);


    void LightSource::updateItem(const glm::mat4& parent_model_matrix) {
        global_light.ambient = light.ambient;
        global_light.diffuse = light.diffuse;
        global_light.specular = light.specular;
        global_light.attenuation = light.attenuation;

        global_light.position = parent_model_matrix * light.position;

        if (light.position.w == 0.0f) {
            global_light.position = glm::vec4(glm::normalize(glm::vec3(global_light.position)), 0.0f);
        }

        if (glm::length(glm::vec3(light.spotlight)) > 0.0f) {
            glm::vec4 world_direction = parent_model_matrix * light.spotlight;
            global_light.spotlight = glm::vec4(glm::normalize(glm::vec3(world_direction)), 0.0f);
        }
        else {
            global_light.spotlight = light.spotlight;
        }
    }


    void LightBlock::addLight(LightSource* LS) {
        if (LBD.numLights >= MAX_LIGHTS) return;

        scene_light.emplace_back(LS);
        LBD.lights[LBD.numLights++] = LS->getGlobalLight();
    }


    void LightBlock::updateViewSpacePositions(const glm::mat4& viewMatrix) {
        for (int i = 0; i < LBD.numLights; ++i) {
            Light currentLight = scene_light[i]->getGlobalLight();
            currentLight.position = viewMatrix * currentLight.position;
            if (currentLight.spotlight != glm::vec4(0.0f))
            {
                currentLight.spotlight = glm::normalize(viewMatrix * currentLight.spotlight);
            }
            LBD.lights[i] = currentLight;
        }
    }
}