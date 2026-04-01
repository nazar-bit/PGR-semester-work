#pragma once
#include <glm/glm.hpp>

namespace vasylnaz {

    struct Material {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;  // specular + shininess in w
        glm::vec4 emission;
    };

}

