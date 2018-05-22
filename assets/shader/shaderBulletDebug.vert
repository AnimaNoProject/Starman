#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 viewProj;

out vec3 color;

void main()
{	
    vec4 position_world = vec4(aPos, 1);

    gl_Position = viewProj * position_world;
}