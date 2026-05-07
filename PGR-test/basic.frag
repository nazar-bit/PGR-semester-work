#version 330
#define MAX_LIGHTS 32

layout (std140) uniform Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;  // specular + shininess in w
    vec4 emission;
};

struct Light {
    vec4 ambient;  // ambient 
    vec4 diffuse;  // diffuse
    vec4 specular; // specular, w: spotlight cutoff
    vec4 position; 
    vec4 attenuation;  // constant, linear, quadratic, w: spotlight exponent
    vec4 spotlight;  // spotlight normalized directio, w: 0.0f
    mat4 lightSpaceMatrix;
};

layout (std140) uniform LightBlock {
    int numLights;              
    Light lights[MAX_LIGHTS];
};


uniform vec3 global_ambient;
uniform sampler2D texSampler;
uniform sampler2D normSampler;
uniform sampler2D emSampler;

// FOG 
uniform bool fog;
vec3 fog_color = vec3(0.8);
uniform float fog_density = 0.2;
float height_fog_density = 0.9;
float floor_level = -0.7;
in float world_height;

in vec2 fg_tex_coords;
in mat3 TBN;

uniform sampler2DArrayShadow shadowMapArray;

in vec4 fg_position;
in vec4 fg_position_model;
out vec4 color;



float ShadowCalculation(mat4 lightSpaceMatrix, int layerIndex, vec3 normal, vec3 lightDir) {

    vec4 fg_position_light = lightSpaceMatrix * fg_position_model;
    vec3 projCoords = fg_position_light.xyz / fg_position_light.w;
    projCoords = projCoords * 0.5 + 0.5;

    float factor = 0.0001;
    float bias = max(factor*10 * (1.0 - dot(normal, lightDir)), factor);
    vec4 shadowCoord = vec4(projCoords.xy, float(layerIndex), projCoords.z - bias);

    float visibility = texture(shadowMapArray, shadowCoord);
    return 1.0 - visibility; 
}



void main() {
    vec3 sum_light = vec3(0.0);
    
    vec3 normMap = texture(normSampler, fg_tex_coords).rgb;
    normMap = normMap * 2.0 - 1.0; 
    vec3 norm = normalize(TBN * normMap); 
    vec3 view_pos = fg_position.xyz;

    // TextureColor
    vec4 tex_clr = texture(texSampler, fg_tex_coords);
    if(tex_clr.a < 0.1){
        discard;
    }
    vec3 tex_color = tex_clr.xyz;

    
    for(int i = 0; i < numLights; ++i) {
        Light cur_light = lights[i];

        vec3 light_dir;
        if (cur_light.position.w == 0.0) {
        light_dir = normalize(cur_light.position.xyz);
        } else {
            light_dir = normalize(cur_light.position.xyz - view_pos);
        }
        // Shadow
        float shadow = ShadowCalculation(cur_light.lightSpaceMatrix, i, norm, light_dir);
        // SpotlightEffect
        float spotlightEffect = 1.0;
        if(cur_light.spotlight != vec4(0.0))
        {
            float cosA = dot(light_dir, -cur_light.spotlight.xyz);
            if(cosA >= cur_light.specular.w){
                spotlightEffect = pow(max(cosA, 0.0), cur_light.attenuation.w);
            } else{
                spotlightEffect = 0.0;
            }
        }
        // AttenuationFactor
        float attenuationFactor = 1.0;
        if(cur_light.position.w != 0.0)
        {
            attenuationFactor = 1.0/(cur_light.attenuation.x + 
                cur_light.attenuation.y * distance(view_pos, cur_light.position.xyz) +
                cur_light.attenuation.z * pow(distance(view_pos, cur_light.position.xyz), 2));

            if(cur_light.spotlight == vec4(0.0)){
                shadow = 0.0;
            }
        }
        // Ambient
        vec3 ambient_ref = ambient.xyz * cur_light.ambient.xyz;
        ambient_ref = ambient_ref * tex_color;
        // Diffuse
        float diffuse_impact = max(dot(norm, light_dir), 0.0);
        vec3 diffuse_ref = diffuse_impact * diffuse.xyz * cur_light.diffuse.xyz;
        diffuse_ref = diffuse_ref * tex_color; 
        // Specular
        float specular_impact = max(dot(2.0 * dot(light_dir, norm) * norm - light_dir, normalize(-view_pos)), 0.0);
        vec3 specular_ref = pow(specular_impact, specular.w) * specular.xyz * cur_light.specular.xyz; 
        
        sum_light += (spotlightEffect * attenuationFactor) * (ambient_ref + (1.0 - shadow)*(diffuse_ref + specular_ref));
    }


    // Emission
    vec3 final_em = emission.xyz;
    vec3 em_tex = texture(emSampler, fg_tex_coords).rgb;
    if(em_tex != vec3(0.0)){
        final_em = em_tex;  
    }


    // Result
    vec3 res = final_em + (global_ambient * tex_color) + sum_light;
    // FOG
    if(fog){
        float height_from_floor = max(world_height - floor_level, 0.0);
        float height_density_modifier = exp(-height_fog_density * height_from_floor);
        float fog_density = fog_density * height_density_modifier;
        float fog_factor = exp(-fog_density * length(view_pos));
        res = res*fog_factor + (1-fog_factor)*fog_color;
    }

    color = vec4(res, diffuse.w);
}

