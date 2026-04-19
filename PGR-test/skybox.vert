#version 330

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 tex_coords;
uniform mat4 VM;
uniform mat4 VN;
uniform mat4 P;

out vec4 fg_position;
out vec2 fg_tex_coords;

void main() {
    mat4 PVM = P * VM;
    gl_Position = PVM * vec4(position, 1.0);
    
    fg_position = VM * vec4(position, 1.0);
    fg_tex_coords = vec2(tex_coords.x, tex_coords.y);
}