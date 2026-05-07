#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Item.hpp"


namespace vasylnaz {

    #define MAX_LIGHTS 32

    extern glm::vec3 GLOBAL_AMBIENT;
   

    enum LightType {
        DIRECTIONAL,
        POINT,
        SPOTLIGHT
    };


    struct Attenuation {
        float constant;
        float linear;
        float quadratic;

        Attenuation(const float& c, const float& l, const float& q)
            : constant(c), linear(l), quadratic(q) {
        }
    };


    struct Spotlight {
        glm::vec3 direction;  // Normalized
        float cutoff;
        float exponent;

        /// @brief 
        /// @param direction 
        /// @param cutoff 
        /// @param exponent 
        Spotlight(const glm::vec3& direction, const float& cutoff, const float& exponent)
            : direction(direction), cutoff(glm::cos(glm::radians(cutoff))), exponent(exponent) {
        }
    };


    extern Spotlight DEFAULT_SPOT;
    extern Attenuation DEFAULT_ATTEN;

    
    struct Light {
        glm::vec4 ambient;   // w bool active
        glm::vec4 diffuse;   // w empty 
        glm::vec4 specular;  // w: spotlight cutoff

        glm::vec4 position;  
        glm::vec4 attenuation;  // w: spotlight exponent
        glm::vec4 spotlight; // direction + w: 0.0f

        glm::mat4 lightSpaceMatrix;
    };



    /// @brief 
    class LightSource : public Item
    {
    public:

        const long light_id;

        /// @brief 
        /// @param LT 
        /// @param ambient 
        /// @param diffuse 
        /// @param specular 
        /// @param position 
        /// @param spotlight 
        /// @param attenuation 
        LightSource(const LightType& LT, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position,
            const Attenuation& attenuation = DEFAULT_ATTEN, const Spotlight& spotlight = DEFAULT_SPOT)
            : light{
                glm::vec4(ambient, 1.0f),  
                glm::vec4(diffuse, 0.0f),  
                glm::vec4(specular, spotlight.cutoff),
                glm::vec4(position, LT == DIRECTIONAL ? 0.0f : 1.0f),
                glm::vec4(attenuation.constant, attenuation.linear, attenuation.quadratic, spotlight.exponent),
                glm::vec4(LT == SPOTLIGHT ? glm::vec4(spotlight.direction, 0.0f) : glm::vec4(0.0f)),
                glm::mat4(1.0f)
            }, light_id(global_light_id++)
        {
            global_light = light;
        }

        /// @brief 
        /// @return 
        const Light& getGlobalLight() const {
            return global_light;
        }
        /// @brief 
        void activateLight() {
            global_light.ambient.w = 1.0f;
        }
        /// @brief 
        void deactivateLight() {
            global_light.ambient.w = 0.0f;
        }

        /// @brief 
        /// @param matrix 
        void setLightSpaceMatrix(const glm::mat4& matrix) {
            light.lightSpaceMatrix = matrix;
            global_light.lightSpaceMatrix = matrix;
        }

        /// @brief 
        /// @param parent_model_matrix 
        void updateItem(const glm::mat4& parent_model_matrix) override;


    private:
        static long global_light_id;
        Light light;
        Light global_light;
    };



    struct LightBlockData {
        int numLights;       
        float padding[3];    
        Light lights[MAX_LIGHTS];
    };


    class LightBlock {
    public:

        LightBlock()
            : LBD{0}
        {
            //
        }

        /// @brief 
        /// @param LS 
        void addLight(LightSource* LS);


        /// @brief 
        /// @param viewMatrix 
        void updateViewSpacePositions(const glm::mat4& viewMatrix);


        const LightBlockData& getLBD() const {
            return LBD;
        }


    private:
        LightBlockData LBD;
        std::vector<LightSource*> scene_light;
    };


}

