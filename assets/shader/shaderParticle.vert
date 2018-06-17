#version 430

layout (location = 0) in vec4 position; 

uniform mat4 view;
uniform mat4 model;

out vData
{
	float TTL1;
} vertex;

void main()
{
	gl_Position = view * model * vec4(position.xyz, 1.0);
	vertex.TTL1 = position.w;
}