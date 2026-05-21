#version 330

uniform vec3 color_fg;

out vec4 color;


void main() {
   
    color = vec4(color_fg, 1.0);
}

