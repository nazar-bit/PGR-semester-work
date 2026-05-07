#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <iostream>

#include "Curve.hpp"
#include "Params.hpp"
#include "Utils.hpp"
#include "BoundingBox.hpp"


namespace vasylnaz {

    struct RenderContext;
    class Node;


    struct ViewPoint {
        glm::vec3 position;
        glm::vec3 target;   // normalized vector
        glm::vec3 up;

        /// @brief 
        /// @param position Position in the World
        /// @param target Normalized vector specifying direction
        /// @param up Up vector
        ViewPoint(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
            : position(position), target(target), up(up)
        {
            //
        }
    };
    


    class Camera {
    public:
        glm::vec3 position;
        glm::vec3 target;   // normalized vector
        glm::vec3 up;
        glm::vec3 camera_target_distance;
        float movement_speed;
        float rotation_angle;
        Curve* current_curve = nullptr;
        std::vector<Curve*> curves;
        float curve_movement_start = -1;
        float curve_speed = 2.0f;
        std::vector<ViewPoint> view_points;
        std::vector<Node*> follow_nodes;
        Node* current_node = nullptr;
        glm::quat rotation = glm::quat(NAN_FL, NAN_FL, NAN_FL, NAN_FL);
        glm::quat rotation_up = glm::quat(NAN_FL, NAN_FL, NAN_FL, NAN_FL);
        glm::vec3 target_before_rotation;
        glm::vec3 up_before_rotation;

        glm::vec2 boundary_x = glm::vec2(-4.85f, 4.85f);
        glm::vec2 boundary_y = glm::vec2(-0.6f, 2.4f);
        glm::vec2 boundary_z = glm::vec2(0.15f, 9.85);
        float x_split = 1.85f;
        float z_split = 3.15f;

        BoundingBox example = {
            glm::vec3(-4.3f, -0.17f, 0.7f),
            0.8f
        };

        /// @brief 
        /// @param position Position in the World
        /// @param target Normalized vector specifying direction
        /// @param up Up vector
        /// @param camera_target_distance How far away are we from the thing we are looking at
        /// @param movement_speed 
        /// @param rotation_angle 
        Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up,
            const glm::vec3& camera_target_distance, const float& movement_speed,
            const float& rotation_angle)
            : position(position), target(target), up(up), camera_target_distance(camera_target_distance),
            movement_speed(movement_speed), rotation_angle(rotation_angle)
        {
            //
        }

        /// @brief Clears the curves vector and reads new from CAMERA_CURVES (params)
        /// @param rq RenderContext& of some scene (F.E. MainScene)
        void updateCurves(RenderContext& rq);

        /// @brief Adds the curve to camera's curves vector
        /// @param curve Curve ptr
        /// @param rq RenderContext& of some scene (F.E. MainScene)
        void addCurve(Curve* curve, RenderContext& rq);

        /// @brief 
        /// @param view_point ViewPoint&
        void moveToViewPoint(ViewPoint& view_point);

        /// @brief 
        /// @param curve Curve*
        void changeCurve(Curve* curve);

        /// @brief 
        /// @param node Node*
        void followNode(Node* node);

        /// @brief Update position, rotation, etc ...
        void update();

        /// @brief Draws View Points if DEBUG is ON
        /// @param shader_program ShaderProgram&
        /// @param view View matrix
        void drawViewPoints(const ShaderProgram& shader_program, const glm::mat4& view);

        /// @brief 
        /// @param render_context RenderContext& of some scene (F.E. MainScene)
        void init(RenderContext& render_context);

        /// @brief 
        /// @param new_pos Vector of the new world position
        void changePosition(const glm::vec3& new_pos);


    private:
        GLuint vao;
        GLuint vbo;
    };
}