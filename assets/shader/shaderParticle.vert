#version 430

in layout (location = 0) vec4 position; 

uniform mat4 view;

out vData
{
	float TTL1;
} vertex;

void main()
{
	gl_Position = view * mat4(1) * vec4(position.xyz, 1.0);
	vertex.TTL1 = position.w;
}