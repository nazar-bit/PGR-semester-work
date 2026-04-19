#include <iostream>
#include <vector>
#include <unordered_map>
#include "pgr.h"

#include "Object.hpp"
#include "ShaderManager.hpp"
#include "Light.hpp"
#include "InputHandler.hpp"
#include "AssetManager.hpp"
#include "SceneGraph.hpp"
#include "Params.hpp"

#include <glm/gtx/rotate_vector.hpp>


namespace vasylnaz {

    ShaderProgram shader_program;
    ShaderProgram skybox_program;
    ShaderProgram pick_prog;


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

    

    InputHandler input_handler;
    SceneGraph scene_graph;




    void init() {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glPointSize(20.0f);


        shader_program.compile_shaders("basic.vert", "basic.frag");
        shader_program.generateUBOs();

        skybox_program.compile_shaders("skybox.vert", "skybox.frag");

        pick_prog.compile_shaders("pick.vert", "pick.frag");
        AssetManager::getInstance().init(shader_program);

       
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


        scene_graph.init(shader_program);
    }


    void draw() {
        View = glm::lookAt(camera.position,
            camera.position - length(camera.camera_target_distance) * camera.target,
            camera.up);
        Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

        scene_graph.render_context.light_block.updateViewSpacePositions(View);
        shader_program.update_light(scene_graph.render_context.light_block.getLBD());

        input_handler.update_camera(camera, pick_prog, scene_graph, View, Projection);
        scene_graph.update();


        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glUseProgram(shader_program.shaderProgram);

        glUniformMatrix4fv(shader_program.positionP, 1, GL_FALSE, glm::value_ptr(Projection));
        glUniform3fv(shader_program.positionGlobalAmb, 1, glm::value_ptr(GLOBAL_AMBIENT));

        // Normal stuff
        for (const auto& obj : scene_graph.render_context.objects[RenderQueue::OPAQUE_MASK]) {
            obj->draw(shader_program, View);
        }


        // Skybox
        glUseProgram(skybox_program.shaderProgram);
        glUniformMatrix4fv(skybox_program.positionP, 1, GL_FALSE, glm::value_ptr(Projection));

        for (const auto& obj : scene_graph.render_context.objects[RenderQueue::SKYBOX]) {
            obj->draw(skybox_program, View);
        }
        

        glUseProgram(shader_program.shaderProgram);

        // Transparent
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (const auto& obj : scene_graph.render_context.objects[RenderQueue::TRANSPARENT_MASK]) {
            obj->draw(shader_program, View);
        }
        glDisable(GL_BLEND);

        

        glutSwapBuffers();
    }


    void timerCallback(int value) {

        

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

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
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



