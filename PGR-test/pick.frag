#version 330
#define MAX_DEPTH 1

uniform float id;
smooth in float depth;
out vec4 outId;

void main()
{
	outId = vec4(id, 1, -depth / MAX_DEPTH, 0);
}

