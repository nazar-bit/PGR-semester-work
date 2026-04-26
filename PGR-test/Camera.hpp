#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "SceneGraph.hpp"
#include "Curve.hpp"


namespace vasylnaz {

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

        void update(float global_time);

        void drawViewPoints(const ShaderProgram& shader_manager, const glm::mat4& view);

        void initViewPoints();


    private:
        GLuint vao;
        GLuint vbo;

    };
}