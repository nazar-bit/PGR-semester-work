#include "AssetManager.hpp"

namespace vasylnaz {

    void AssetManager::init(ShaderManager shader_manager) {
        diffuse_map_location = shader_manager.positionDiffuseMap;

        loadTetxure("thermos", "Textures/plastic_thermos.png");
        loadTetxure("blank", "Textures/blank.png");

        loadMesh("cube", "Models/cube.obj", shader_manager);
        loadMesh("thermos", "Models/plastic_thermos_1k.obj", shader_manager);


        Material redPlastic;
        redPlastic.ambient = glm::vec4(0.5f, 0.0f, 0.0f, 0.0f);
        redPlastic.diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        redPlastic.specular = glm::vec4(1.0f, 1.0f, 1.0f, 100.0f);
        redPlastic.emission = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        Material basic;
        basic.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f);
        basic.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        basic.specular = glm::vec4(1.0f, 1.0f, 1.0f, 64.0f);
        basic.emission = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        loadMaterial("red_plastic", redPlastic);
        loadMaterial("basic", basic);
    }
}