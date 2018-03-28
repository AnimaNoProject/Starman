#version 430

out vec4 FragColor;

uniform vec3 materialCoefficients; // x = ambient, y = diffuse, z = specular 
uniform float specularAlpha;

void main(void) {
	FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}