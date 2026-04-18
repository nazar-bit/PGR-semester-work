#include "AssetManager.hpp"

namespace vasylnaz {

    void AssetManager::init(ShaderProgram& shader_manager) {
        diffuse_map_location = shader_manager.positionDiffuseMap;
        //diffuse
        loadTetxure("thermos", "Textures/plastic_thermos.png");
        loadTetxure("blank", "Textures/blank.png");
        loadTetxure("rocks", "Textures/gray_rocks.png");
        //loadTetxure("whitewashed_brick", "Textures/brick_wall_10_diff_1k.jpg");
        loadTetxure("whitewashed_brick", "Textures/Bricks038_1K-JPG_Color.jpg");
        loadTetxure("floor", "Textures/granite_tile_diff_1k.jpg");
        loadTetxure("wall_support", "Textures/wooden_gate_diff_1k.jpg");
        loadTetxure("ceiling", "Textures/OfficeCeiling001_1K-JPG_Color.jpg");
        
        
        
        
        //normal
        loadTetxure("blank_norm", "Textures/blank_norm.png");
        loadTetxure("rocks_norm", "Textures/normalMapRocks.png");
        loadTetxure("thermos_norm", "Textures/normalThermos.png");
        /*loadTetxure("whitewashed_brick_norm", "Textures/brick_wall_10_nor_gl_1k.jpg");*/
        loadTetxure("whitewashed_brick_norm", "Textures/Bricks038_1K-JPG_NormalGL.jpg");
        loadTetxure("floor_norm", "Textures/granite_tile_nor_gl_1k.jpg");
        loadTetxure("wall_support_norm", "Textures/wooden_gate_nor_gl_1k.jpg");
        loadTetxure("ceiling_norm", "Textures/OfficeCeiling001_1K-JPG_NormalGL.jpg");

        //emission
        loadTetxure("blank_em", "Textures/blank_em.png");
        

        loadMesh("cube", "Models/cube.obj", shader_manager);
        loadMesh("thermos", "Models/plastic_thermos_1k.obj", shader_manager);
        loadMesh("ground", "Models/ground.obj", shader_manager);
        loadMesh("wall_with_holes", "Models/wall_with_holes.obj", shader_manager);


        Material redPlastic;
        redPlastic.ambient = glm::vec4(0.5f, 0.0f, 0.0f, 0.0f);
        redPlastic.diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        redPlastic.specular = glm::vec4(1.0f, 1.0f, 1.0f, 100.0f);
        redPlastic.emission = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        loadMaterial("red_plastic", redPlastic);

        Material basic;
        basic.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f);
        basic.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        basic.specular = glm::vec4(1.0f, 1.0f, 1.0f, 64.0f);
        basic.emission = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        loadMaterial("basic", basic);

        Material semi_trans;
        semi_trans.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f);
        semi_trans.diffuse = glm::vec4(0.1f, 0.1f, 0.1f, 0.3f);
        semi_trans.specular = glm::vec4(1.0f, 1.0f, 1.0f, 64.0f);
        semi_trans.emission = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        loadMaterial("semi_trans", semi_trans);
    }


    void AssetManager::loadTetxure(const string& name, const string& file_path) {
        auto it = textures.find(name);

        if (it != textures.end()) {
            std::cout << "Texture was already loaded" << "\n";
        }
        else {
            GLuint texID = pgr::createTexture(file_path);
            textures.emplace(name, texID);
            std::cout << "Successfully loaded: " << file_path << std::endl;
        }
    }


    void AssetManager::loadMaterial(const string& name, const Material& material) {
        auto it = materials.find(name);

        if (it != materials.end()) {
            std::cout << "Material was already loaded" << "\n";
        }
        else {
            materials.emplace(name, std::make_unique<Material>(material));
            std::cout << "Successfully loaded: " << name << std::endl;
        }
    }


    void AssetManager::loadMesh(const string& name, const string& file_path, ShaderProgram& shader_manager) {
        auto it = meshes.find(name);

        if (it != meshes.end()) {
            std::cout << "Mesh was already loaded" << "\n";
        }
        else {
            meshes.emplace(name, std::make_unique<Mesh>(file_path, shader_manager));
        }
    }


    void AssetManager::loadMesh(const string& name, std::unique_ptr<Mesh> mesh) {
        auto it = meshes.find(name);

        if (it != meshes.end()) {
            std::cout << "Mesh was already loaded" << "\n";
        }
        else {
            meshes.emplace(name, std::move(mesh));
        }
    }
}