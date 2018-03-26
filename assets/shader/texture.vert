#version 430

layout (location = 0) in vec3 in_Position;

uniform mat4 view_projection;

void main()
{
	gl_Position = view_projection * vec4(in_Position, 1.0);
}