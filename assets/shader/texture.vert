#version 430
layout (location = 0) in vec3 in_position;

uniform mat4 view_projection;
uniform mat4 gTransformation;

void main()
{
	gl_Position = view_projection * (gTransformation * vec4(in_position, 1.0));
};