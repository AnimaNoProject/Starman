#version 430 core

layout (location = 0) in vec3 aPos;

uniform mat4 viewProj;

void main()
{	
    vec4 position_world = vec4(aPos, 1);

    gl_Position = viewProj * position_world;
}