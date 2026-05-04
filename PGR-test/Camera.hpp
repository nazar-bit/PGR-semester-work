#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <iostream>

#include "Curve.hpp"
#include "Params.hpp"
#include "Utils.hpp"


namespace vasylnaz {

    struct RenderContext;
    class Node;


    struct ViewPoint {
        glm::vec3 position;
        glm::vec3 target;   // normalized vector
        glm::vec3 up;

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

        /// @brief 
        /// @param position 
        /// @param target 
        /// @param up 
        /// @param camera_target_distance 
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

        void updateCurves(RenderContext& rq);

        void addCurve(Curve* curve, RenderContext& rq);

        void moveToViewPoint(ViewPoint& view_point);

        void changeCurve(Curve* curve);

        void followNode(Node* node);

        void update();

        void drawViewPoints(const ShaderProgram& shader_manager, const glm::mat4& view);

        void init(RenderContext& render_context);

        void changePosition(const glm::vec3& new_pos);


    private:
        GLuint vao;
        GLuint vbo;
    };
}