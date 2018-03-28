#version 430

layout (location = 0) in vec3 position;

uniform mat4 modelMatrix;
uniform mat4 viewProjMatrix;

void main()
{
    vec4 position_world = modelMatrix * vec4(position, 1);
	gl_Position = viewProjMatrix * position_world;
}