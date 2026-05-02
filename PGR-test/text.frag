#version 330

uniform sampler2D texSampler;
uniform vec3 textColor;

in vec2 fg_tex_coords;

out vec4 color;


void main() {
   
    vec4 tex_clr = texture(texSampler, fg_tex_coords);
    if(tex_clr.xyz == vec3(0.0)){
        discard;
    }
    color = vec4(textColor, 1.0);
}

