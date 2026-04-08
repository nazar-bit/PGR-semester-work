#include <iostream>
#include <vector>
#include <unordered_map>
#include "pgr.h"

#include "Object.hpp"
#include "ShaderManager.hpp"
#include "Light.hpp"
#include "InputHandler.hpp"
#include "AssetManager.hpp"

#include <glm/gtx/rotate_vector.hpp>


namespace vasylnaz {
    long Object::global_object_id = 0;
    long Mesh::global_mesh_id = 0;
    long LightSource::global_light_id = 0;

    const int WIN_WIDTH = 1024;
    const int WIN_HEIGHT = 768;
    const char* WIN_TITLE = "Hello World";


    // Mesh Id lets you access objects sharing this mesh
    std::unordered_map<long, std::vector<Object>> scene_objects;

    ShaderManager shader_manager;
    AssetManager asset_manager;


    Camera camera = Camera(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),   // normalized vector
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 5.0f),
        0.05f,
        0.2f
    );
    

    glm::mat4 View;
    glm::mat4 Projection;

    LightBlock light_block;

    InputHandler input_handler;




    void init() {
        glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glPointSize(20.0f);

        shader_manager.compile_shaders();
        shader_manager.generateUBOs();

        asset_manager.diffuse_map_location = shader_manager.positionDiffuseMap;
        asset_manager.loadTetxure("thermos", "Textures/plastic_thermos.png");
        asset_manager.loadTetxure("blank", "Textures/blank.png");

       
        const float vertices[] = {
            // Front face
            -0.5f, -0.5f,  0.5f,  // 0: Bottom-left
             0.5f, -0.5f,  0.5f,  // 1: Bottom-right
             0.5f,  0.5f,  0.5f,  // 2: Top-right
            -0.5f,  0.5f,  0.5f,  // 3: Top-left

            // Back face
             0.5f, -0.5f, -0.5f,  // 4: Bottom-left (facing back)
            -0.5f, -0.5f, -0.5f,  // 5: Bottom-right
            -0.5f,  0.5f, -0.5f,  // 6: Top-right
             0.5f,  0.5f, -0.5f,  // 7: Top-left

             // Left face
             -0.5f, -0.5f, -0.5f,  // 8: Bottom-left
             -0.5f, -0.5f,  0.5f,  // 9: Bottom-right
             -0.5f,  0.5f,  0.5f,  // 10: Top-right
             -0.5f,  0.5f, -0.5f,  // 11: Top-left

             // Right face
              0.5f, -0.5f,  0.5f,  // 12: Bottom-left
              0.5f, -0.5f, -0.5f,  // 13: Bottom-right
              0.5f,  0.5f, -0.5f,  // 14: Top-right
              0.5f,  0.5f,  0.5f,  // 15: Top-left

              // Top face
              -0.5f,  0.5f,  0.5f,  // 16: Bottom-left
               0.5f,  0.5f,  0.5f,  // 17: Bottom-right
               0.5f,  0.5f, -0.5f,  // 18: Top-right
              -0.5f,  0.5f, -0.5f,  // 19: Top-left

              // Bottom face
              -0.5f, -0.5f, -0.5f,  // 20: Bottom-left
               0.5f, -0.5f, -0.5f,  // 21: Bottom-right
               0.5f, -0.5f,  0.5f,  // 22: Top-right
              -0.5f, -0.5f,  0.5f   // 23: Top-left
        };

        // 24 normals (nx, ny, nz) - 72 floats total
        const float normals[] = {
            // Front face (Normal pointing towards +z)
            0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
            // Back face (Normal pointing towards -z)
            0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            // Left face (Normal pointing towards -x)
            -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            // Right face (Normal pointing towards +x)
            1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
            // Top face (Normal pointing towards +y)
            0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
            // Bottom face (Normal pointing towards -y)
            0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f
        };

        // 36 indices defining the 12 triangles
        const unsigned int indices[] = {
             0,  1,  2,   2,  3,  0,  // Front
             4,  5,  6,   6,  7,  4,  // Back
             8,  9, 10,  10, 11,  8,  // Left
            12, 13, 14,  14, 15, 12,  // Right
            16, 17, 18,  18, 19, 16,  // Top
            20, 21, 22,  22, 23, 20   // Bottom
        };


        const LightSource testLight(
            POINT,
            glm::vec3(0.1f),     //amb
            glm::vec3(2.0f),     //diff
            glm::vec3(1.0f),    //spec
            glm::vec3(2.0f, 2.0f, 5.0f),    //pos
            Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f),
            Attenuation(1.0f, 0.09f, 0.032f)
        );


        const LightSource testLight2(
            POINT,
            glm::vec3(0.1f),     //amb
            glm::vec3(2.0f),     //diff
            glm::vec3(1.0f),    //spec
            glm::vec3(6.0f, 2.0f, 5.0f),    //pos
            Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f),
            Attenuation(1.0f, 0.09f, 0.032f)
        );


        const LightSource testLight3(
            POINT,
            glm::vec3(0.1f),     //amb
            glm::vec3(2.0f),     //diff
            glm::vec3(1.0f),    //spec
            glm::vec3(6.0f, 2.0f, 8.0f),    //pos
            Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f),
            Attenuation(1.0f, 0.09f, 0.032f)
        );

        

        /*const LightSource testLight2(
            glm::vec3(0.1f),
            glm::vec3(0.8f),
            glm::vec3(1.0f),
            glm::vec3(0.0f, 15.0f, 0.0f),
            1.0f,
            1.0f
        );*/

        light_block.addLight(testLight);
        light_block.addLight(testLight2);
        light_block.addLight(testLight3);


        /*scene_meshes.emplace_back(std::make_unique<Mesh>(vertices, sizeof(vertices) / sizeof(float), normals,
            indices, sizeof(indices) / sizeof(unsigned short), shader_manager));*/

        asset_manager.loadMesh("cube", "Models/cube.obj", shader_manager);
        asset_manager.loadMesh("thermos", "Models/plastic_thermos_1k.obj", shader_manager);


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

        asset_manager.loadMaterial("red_plastic", redPlastic);
        asset_manager.loadMaterial("basic", basic);

        auto test_object = Object(asset_manager, "thermos", glm::mat4(1.0f), "basic", "thermos");
        auto model_mat2 = glm::mat4(
            5.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 5.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 5.0f, 0.0f,
            5.0f, 0.0f, 0.0f, 1.0f);
        auto test_object2 = Object(asset_manager, "cube", model_mat2, "basic", "blank");

        auto model_mat3 = glm::mat4(
            100.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.1f, 0.0f, 0.0f,
            0.0f, 0.0f, 100.0f, 0.0f,
            0.0f, -3.0f, 0.0f, 1.0f);
        auto test_object3 = Object(asset_manager, "cube", model_mat3, "red_plastic");

        scene_objects[test_object.get_mesh()->mesh_id].emplace_back(std::move(test_object));
        scene_objects[test_object2.get_mesh()->mesh_id].emplace_back(std::move(test_object2));
        scene_objects[test_object3.get_mesh()->mesh_id].emplace_back(std::move(test_object3));
    }


    void draw() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_manager.shaderProgram);

        View = glm::lookAt(camera.position,
            camera.position - length(camera.camera_target_distance) * camera.target,
            camera.up);
        Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(shader_manager.positionP, 1, GL_FALSE, glm::value_ptr(Projection));

        light_block.updateViewSpacePositions(View);
        shader_manager.update_light(light_block.getLBD());

        glUniform3fv(shader_manager.positionGlobalAmb, 1, glm::value_ptr(GLOBAL_AMBIENT));

        for (const auto& array : scene_objects) {
            for (const auto& obj : array.second) {
                obj.draw(shader_manager, View);
            }
        }

        glutSwapBuffers();
    }


    void timerCallback(int value) {

        input_handler.update_camera(camera);

        glutPostRedisplay();
        glutTimerFunc(16, timerCallback, 0);
    }


    void handleKeyboardDown(unsigned char key, int x, int y) {
        input_handler.set_key_true(key);
    }


    void handleKeyboardUp(unsigned char key, int x, int y) {
        input_handler.set_key_false(key);
    }


    void handleMouseClick(int button, int state, int x, int y) {
        input_handler.handle_mouse_click(button, state, x, y);
    }


    void handleMouseDrag(int x, int y) {
        input_handler.handle_mouse_drag(x, y);
    }


    void handleSpecialKeysDown(int key, int x, int y) {
        input_handler.set_special_key_true(key);
    }


    void handleSpecialKeysUp(int key, int x, int y) {
        input_handler.set_special_key_false(key);
    }

}


using namespace vasylnaz;

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow(WIN_TITLE);

    glutDisplayFunc(draw);

    if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
        pgr::dieWithError("pgr init failed, required OpenGL not supported?");

    init();

    std::cout << "Hello triangle!" << std::endl;

    glutKeyboardFunc(handleKeyboardDown);
    glutKeyboardUpFunc(handleKeyboardUp);
    glutSpecialFunc(handleSpecialKeysDown);
    glutSpecialUpFunc(handleSpecialKeysUp);
    glutMouseFunc(handleMouseClick);
    glutMotionFunc(handleMouseDrag);

    glutTimerFunc(16, timerCallback, 0);
    glutMainLoop();
    return 0;
}



