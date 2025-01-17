#version 430 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube sky;

void main()
{    
    FragColor = texture(sky, TexCoords);
}