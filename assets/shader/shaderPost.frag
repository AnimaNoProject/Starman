#version 430 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;

void main()
{    
    color = 1-texture(renderedTexture, UV).xyz;
}
















