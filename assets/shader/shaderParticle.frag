#version 430 core

in float color;

flat in float TTL0;
out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}	