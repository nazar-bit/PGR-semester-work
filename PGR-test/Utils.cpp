#include "Utils.hpp"



namespace vasylnaz {


    glm::quat calculateRotation(glm::vec3 u, glm::vec3 v) {

        float angle = 0;
        float denominator = glm::length(u) * glm::length(v);
        if (denominator != 0) {
            angle = glm::acos(glm::dot(u, v) / denominator) / 2.0f;
        }
        denominator = glm::length(glm::cross(u, v));

        glm::vec3 w = glm::vec3(0.0f);
        if (denominator != 0) {
            w = glm::cross(u, v) / denominator;
        }

        return glm::quat(glm::cos(angle), w * glm::sin(angle));
    }



    glm::quat calculateRotationCurve(glm::vec3 oldPos, glm::vec3 newPos) {

        glm::vec3 diff = newPos - oldPos;
        if (glm::length(diff) < 0.0001f) {
            return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        }

        glm::vec3 forwardDir = glm::normalize(newPos - oldPos);
        glm::vec3 backwardDir = -forwardDir;
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 defaultBackward = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 defaultUp = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::quat q_nose = glm::rotation(defaultBackward, backwardDir);
        glm::vec3 currentUp = q_nose * defaultUp;

        glm::vec3 rightDir = glm::normalize(glm::cross(backwardDir, worldUp));
        glm::vec3 perfectUp = glm::normalize(glm::cross(rightDir, backwardDir));

        glm::quat q_roll = glm::rotation(currentUp, perfectUp);

        return glm::normalize(q_roll * q_nose);
    }


    float GlobalTime = 0.0f;
    float PauseStart = 0.0f;
    float PauseAdjustment = 0.0f;

    float NAN_FL = std::numeric_limits<float>::quiet_NaN();

    glm::vec3 defaultBackward = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 defaultUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 defaultRight = glm::vec3(1.0f, 0.0f, 0.0f);

}