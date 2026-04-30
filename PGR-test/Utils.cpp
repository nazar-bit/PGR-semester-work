#include "Utils.hpp"



namespace vasylnaz {


    glm::quat calculateRotation(glm::vec3 oldPos, glm::vec3 newPos) {

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

}