#version 420 core

in layout (location = 0) vec3 position; 
in layout (location = 1) float life;

uniform mat4 viewProj;

out float color;

void main()
{
	gl_Position = viewProj * vec4(position.xyz, 1.0);
	color = life;
}