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

        /// @brief 
        /// @param constant Constant attenuation factor
        /// @param linear Linear attenuation factor
        /// @param quadratic Quadratic attenuation factor
        Attenuation(const float& constant, const float& linear, const float& quadratic)
            : constant(constant), linear(linear), quadratic(quadratic) {
        }
    };


    struct Spotlight {
        glm::vec3 direction;  // Normalized
        float cutoff;
        float exponent;

        /// @brief 
        /// @param direction Normalized direction
        /// @param cutoff Cutoff factor in angles (What happens: glm::cos(glm::radians(cutoff)))
        /// @param exponent Exponent 
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




    class LightSource : public Item
    {
    public:

        const long light_id;

        /// @brief 
        /// @param LT Light Type (F.E. Spotlight)
        /// @param ambient Ambient param
        /// @param diffuse Diffuse param
        /// @param specular Specular param
        /// @param position World position (Direction for DIRECTIONAL light)
        /// @param attenuation Attenuation& (Ignored for Dirrectional)
        /// @param spotlight Spotlight& (Ignored for everyone that is not a SPOTLIGHT type)
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
        /// @return Global light
        const Light& getGlobalLight() const {
            return global_light;
        }

        /// @brief Show light on the scene
        void activateLight() {
            global_light.ambient.w = 1.0f;
        }

        /// @brief Hide light on the scene
        void deactivateLight() {
            global_light.ambient.w = 0.0f;
        }

        /// @brief 
        /// @param matrix LightSpaceMatrix
        void setLightSpaceMatrix(const glm::mat4& matrix) {
            light.lightSpaceMatrix = matrix;
            global_light.lightSpaceMatrix = matrix;
        }

        /// @brief Update the light based on the Parent's model matrix in SceneGraph 
        /// @param parent_model_matrix Parent's model matrix 
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

        /// @brief 
        LightBlock()
            : LBD{0}
        {
            //
        }

        /// @brief 
        /// @param LS LightSource*
        void addLight(LightSource* LS);

        /// @brief Update light's position in view space
        /// @param viewMatrix View matrix
        void updateViewSpacePositions(const glm::mat4& viewMatrix);

        /// @brief 
        /// @return LightBlockData
        const LightBlockData& getLBD() const {
            return LBD;
        }


    private:
        LightBlockData LBD;
        std::vector<LightSource*> scene_light;
    };
}

