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
#include "Curve.hpp"
#include "Utils.hpp"

#include <glm/gtx/rotate_vector.hpp>


namespace vasylnaz {

    ShaderProgram shader_program;
    ShaderProgram skybox_program;
    ShaderProgram pick_prog;
    ShaderProgram leaf_program;
    ShaderProgram tv_program;
    ShaderProgram line_drawer;


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

        // Compile Shaders
        initializeSharedUBOs();

        shader_program.compile_shaders("basic.vert", "basic.frag");
        shader_program.bindUBOs();

        skybox_program.compile_shaders("skybox.vert", "skybox.frag");

        leaf_program.compile_shaders("leaf.vert", "basic.frag");
        leaf_program.bindUBOs();

        tv_program.compile_shaders("basic.vert", "tv.frag");
        tv_program.bindUBOs();

        line_drawer.compile_shaders("line.vert", "line.frag");

        pick_prog.compile_shaders("pick.vert", "pick.frag");
        AssetManager::getInstance().init();

      
        camera.init(scene_graph.render_context);
        scene_graph.init(shader_program, camera);
    }


    void draw() {
        GlobalTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

        View = glm::lookAt(camera.position,
            camera.position - length(camera.camera_target_distance) * camera.target,
            camera.up);
        Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

        scene_graph.render_context.light_block.updateViewSpacePositions(View);
        updateLights(scene_graph.render_context.light_block.getLBD());

        scene_graph.update();
        input_handler.update_camera(camera, pick_prog, scene_graph, View, Projection);
        camera.update();



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


        glDisable(GL_CULL_FACE);
        // No Culling
        for (const auto& obj : scene_graph.render_context.objects[RenderQueue::NO_CULLING_MASK]) {
            obj->draw(shader_program, View);
        }

        // Leafs
        glUseProgram(leaf_program.shaderProgram);
        glUniform1f(leaf_program.positionTime, GlobalTime);
        glUniformMatrix4fv(leaf_program.positionP, 1, GL_FALSE, glm::value_ptr(Projection));
        glUniform3fv(leaf_program.positionGlobalAmb, 1, glm::value_ptr(GLOBAL_AMBIENT));

        for (const auto& obj : scene_graph.render_context.objects[RenderQueue::LEAF_MASK]) {
            obj->draw(leaf_program, View);
        }


        if (!HIDE_DEBUG)
        {
            // Curves
            glUseProgram(line_drawer.shaderProgram);
            glUniform3fv(line_drawer.positionColor, 1, glm::value_ptr(debugColor));
            glUniformMatrix4fv(line_drawer.positionP, 1, GL_FALSE, glm::value_ptr(Projection));
            for (const auto& curve : scene_graph.render_context.curves) {
                curve->draw(line_drawer, View);
            }

            //Points
            glUniform3fv(line_drawer.positionColor, 1, glm::value_ptr(pointDebugColor));
            camera.drawViewPoints(line_drawer, View);

            glEnable(GL_CULL_FACE);
        }
        


        // TV screen
        glUseProgram(tv_program.shaderProgram);
        glUniform1f(tv_program.positionTime, GlobalTime);
        glUniformMatrix4fv(tv_program.positionP, 1, GL_FALSE, glm::value_ptr(Projection));
        glUniform3fv(tv_program.positionGlobalAmb, 1, glm::value_ptr(GLOBAL_AMBIENT));

        for (const auto& obj : scene_graph.render_context.objects[RenderQueue::TV_SCREEN_MASK]) {
            obj->draw(tv_program, View);
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



