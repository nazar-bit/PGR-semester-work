#pragma once

#include <glm/glm.hpp>
#include "SceneGraph.hpp"


namespace vasylnaz {
    class Camera {
    public:
        glm::vec3 position;
        glm::vec3 target;   // normalized vector
        glm::vec3 up;
        glm::vec3 camera_target_distance;
        float movement_speed;
        float rotation_angle;

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


    private:

    };

   
}