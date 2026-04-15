#version 140

in vec3 position;
in vec3 normal;
in vec3 tex_coords;
in vec3 tangent;
uniform mat4 VM;
uniform mat4 VN;
uniform mat4 P;

out vec4 fg_position;
out vec2 fg_tex_coords;
out mat3 TBN;

void main() {
 mat4 PVM = P * VM;
    gl_Position = PVM * vec4(position, 1.0);
    
    fg_position = VM * vec4(position, 1.0);
    fg_tex_coords = vec2(tex_coords.x, tex_coords.y);

    vec3 T = normalize(vec3(VN * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(VN * vec4(normal, 0.0)));
    
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
}