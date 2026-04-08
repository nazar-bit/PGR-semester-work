#pragma once
#include <glm/glm.hpp>
#include <vector>


namespace vasylnaz {

    #define MAX_LIGHTS 10

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
        Spotlight(const glm::vec3& d, const float& c, const float& e)
            : direction(d), cutoff(glm::cos(glm::radians(c))), exponent(e) {
        }
    };


    struct Light {
        glm::vec4 ambient;   // w empty
        glm::vec4 diffuse;   // w empty 
        glm::vec4 specular;  // w: spotlight cutoff

        glm::vec4 position;  
        glm::vec4 attenuation;  // w: spotlight exponent
        glm::vec4 spotlight; // direction + w: 0.0f
    };



    /// @brief 
    class LightSource 
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
            const Spotlight& spotlight, const Attenuation& attenuation)
            : light{
                glm::vec4(ambient, 0.0f),  
                glm::vec4(diffuse, 0.0f),  
                glm::vec4(specular, spotlight.cutoff),
                glm::vec4(position, LT == DIRECTIONAL ? 0.0f : 1.0f),
                glm::vec4(attenuation.constant, attenuation.linear, attenuation.quadratic, spotlight.exponent),
                glm::vec4(LT == SPOTLIGHT ? glm::vec4(spotlight.direction, 0.0f) : glm::vec4(0.0f)),
            }, light_id(global_light_id++)
        {
            //
        }


        const Light& getLight() const {
            return light;
        }


    private:
        static long global_light_id;
        Light light;
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


        void addLight(const LightSource& LS) {
            if (LBD.numLights >= MAX_LIGHTS) return;

            scene_light.emplace_back(LS);
            LBD.lights[LBD.numLights++] = LS.getLight();
        }


        void updateViewSpacePositions(const glm::mat4& viewMatrix) {
            for (int i = 0; i < LBD.numLights; ++i) {
                Light currentLight = scene_light[i].getLight();
                currentLight.position = viewMatrix * currentLight.position;
                if (currentLight.spotlight != glm::vec4(0.0f))
                {
                    currentLight.spotlight = glm::normalize(viewMatrix * currentLight.spotlight);
                }
                LBD.lights[i] = currentLight;
            }
        }


        const LightBlockData& getLBD() const {
            return LBD;
        }


    private:
        LightBlockData LBD;
        std::vector<LightSource> scene_light;
    };


}

