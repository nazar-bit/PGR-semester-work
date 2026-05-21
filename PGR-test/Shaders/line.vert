#version 330

layout(location = 0) in vec3 position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
    mat4 PVM = P * V * M;
    gl_Position = PVM * vec4(position, 1.0);
}

