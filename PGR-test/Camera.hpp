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

    extern glm::vec3 pointDebugColor;


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
        std::vector<std::unique_ptr<Curve>> curves;
        float curve_movement_start = -1;
        float curve_speed = 2.0f;
        std::vector<ViewPoint> view_points;
        std::vector<Node*> follow_nodes;
        Node* current_node = nullptr;
        glm::quat rotation = glm::quat(NAN_FL, NAN_FL, NAN_FL, NAN_FL);
        glm::quat rotation_up = glm::quat(NAN_FL, NAN_FL, NAN_FL, NAN_FL);
        glm::vec3 target_before_rotation;
        glm::vec3 up_before_rotation;

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

        void addCurve(std::unique_ptr<Curve> curve, RenderContext& rq);

        void moveToViewPoint(ViewPoint& view_point);

        void changeCurve(Curve* curve);

        void followNode(Node* node);

        void update();

        void drawViewPoints(const ShaderProgram& shader_manager, const glm::mat4& view);

        void init(RenderContext& render_context);


    private:
        GLuint vao;
        GLuint vbo;
    };
}