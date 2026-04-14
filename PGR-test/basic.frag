#version 140
#define MAX_LIGHTS 10

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
};

layout (std140) uniform LightBlock {
    int numLights;              
    Light lights[MAX_LIGHTS];
};


uniform vec3 global_ambient;
uniform sampler2D texSampler;

in vec2 fg_tex_coords;

in vec4 fg_normal;
in vec4 fg_position;
out vec4 color;


void main() {
    vec3 sum_light = vec3(0.0);
    
    vec3 norm = normalize(fg_normal.xyz);
    vec3 view_pos = fg_position.xyz;

    // TextureColor
    vec4 tex_clr = texture(texSampler, fg_tex_coords);
    if(tex_clr.a < 0.1){
        discard;
    }
    if (tex_clr.r < 0.1 && tex_clr.g < 0.1 && tex_clr.b < 0.1) {
        discard;
    }
    vec3 tex_color = tex_clr.xyz;

    
    
    for(int i = 0; i < numLights; ++i) {
        Light cur_light = lights[i];
        vec3 light_dir = normalize(cur_light.position.xyz - view_pos);
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
        
        sum_light += (spotlightEffect * attenuationFactor) * (ambient_ref + diffuse_ref + specular_ref);
    }

    vec3 res = emission.xyz + (global_ambient * tex_color) + sum_light;
    color = vec4(res, 1.0);
    //color = texture(texSampler, fg_tex_coords);
}

