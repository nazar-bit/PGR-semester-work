#version 330
uniform mat4 M; 
uniform mat4 V; 
uniform mat4 P; 
layout(location = 0) in vec3 position; 

smooth out float depth; 

void main()
{
	mat4 VM = V*M;
	vec4 pos4 = VM * vec4(position, 1); 
	gl_Position = P * pos4; 
	depth = pos4.z; 
}

