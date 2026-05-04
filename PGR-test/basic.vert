#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tex_coords;
layout(location = 3) in vec3 tangent;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec4 fg_position;
out vec2 fg_tex_coords;
out mat3 TBN;

out float world_height;

void main() {
    mat4 VM = V*M;
    mat4 VN = transpose(inverse(V*M));
    mat4 PVM = P * VM;
    gl_Position = PVM * vec4(position, 1.0);
    
    fg_position = VM * vec4(position, 1.0);
    //fg_tex_coords = vec2(tex_coords.x, tex_coords.y);

    float scaleX = length(vec3(VM[0]));
    float scaleZ = length(vec3(VM[2]));
    fg_tex_coords = vec2(tex_coords.x * scaleX, tex_coords.y * scaleZ);

    vec3 T = normalize(vec3(VN * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(VN * vec4(normal, 0.0)));
    
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    world_height = (M * vec4(position, 1.0)).y;
}