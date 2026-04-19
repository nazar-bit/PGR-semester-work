#version 330
uniform mat4 VM; 
uniform mat4 P; 
layout(location = 0) in vec3 position; 

smooth out float depth; 

void main()
{
	vec4 pos4 = VM * vec4(position, 1); 
	gl_Position = P * pos4; 
	depth = pos4.z; 
}

