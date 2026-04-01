#version 140

in vec3 position;
in vec3 normal;
uniform mat4 VM;
uniform mat4 VN;
uniform mat4 P;

out vec4 fg_normal;
out vec4 fg_position;

void main() {
  mat4 PVM = P * VM;
  gl_Position = PVM * vec4(position, 1.0);
  fg_normal = VN * vec4(normal, 0.0);
  fg_position = VM * vec4(position, 1.0);
  
}