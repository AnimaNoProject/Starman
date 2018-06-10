#version 430 core

layout (points) in;
layout(points, max_vertices=4) out;

uniform mat4 viewProj;

in vData { float TTL1; } vertex[];

flat out float TTL0;

void main(void)
{
	vec4 position = gl_in[0].gl_Position;

	gl_Position =  position + vec4(-1, -1, 0, 0);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

	gl_Position = position + vec4(1, -1, 0, 0);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

	gl_Position = position + vec4(-1, 1, 0, 0);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

	gl_Position = position + vec4(1, 1, 0, 0);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

    EndPrimitive();
}