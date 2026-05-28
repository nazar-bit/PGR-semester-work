#version 330

uniform sampler2D texSampler;

in vec2 fg_tex_coords;

out vec4 color;


void main() {
   
    vec4 tex_clr = texture(texSampler, fg_tex_coords);     
    color = tex_clr;
}

