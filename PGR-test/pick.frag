#version 330
#define MAX_DEPTH 1

uniform int id;
smooth in float depth;
out vec4 outId;

void main()
{
	float partOne = float(id & 0x00FF) / 255.0;
	float partTwo = float((id >> 8) & 0x00FF) / 255.0;
	outId = vec4(partOne, 1, -depth / MAX_DEPTH, partTwo);
}

