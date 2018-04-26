#version 430 core

uniform sampler2D texture_diff;

in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 texture_coord;
} vert;

out vec4 color;

void main()
{    
	vec3 b_color = texture(texture_diff, vert.texture_coord).rgb;
	b_color = b_color * 0.9; // ambient, replace 1 with material kA

	color = vec4(b_color, 1);
}