#version 430

out vec4 FragColor;
layout (location = 1) uniform vec3 in_Color;

void main(void) {
	FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}