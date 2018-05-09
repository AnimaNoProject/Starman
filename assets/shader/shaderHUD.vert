#version 430 core

in vec4 vert;
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vert.xy, 0.0, 1.0);
    TexCoords = vert.zw;
}  