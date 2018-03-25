#version 430

layout (location = 0) in vec3 in_Position;

uniform mat4 view_projection;
uniform mat4 gTransformation;

void main()
{

	gl_Position = view_projection * (gTransformation * vec4(in_Position, 1.0));

};