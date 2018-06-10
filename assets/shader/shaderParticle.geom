#version 430 core

layout (points) in;
layout(points, max_vertices=4) out;

uniform mat4 viewProj;

in vData { float TTL1; } vertex[];

flat out float TTL0;

void main(void)
{
	const vec2 size = vec2(5, 5);

	vec4 P = gl_in[0].gl_Position;
	vec2 va = P.xy + vec2(-0.5, -0.5) * size;
	gl_Position = viewProj * vec4(va, P.zw);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

	va = P.xy + vec2(0.5, -0.5) * size;
	gl_Position = viewProj * vec4(va, P.zw);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

	va = P.xy + vec2(0.5, 0.5) * size;
	gl_Position = viewProj * vec4(va, P.zw);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

	va = P.xy + vec2(-0.5, 0.5) * size;
	gl_Position = viewProj * vec4(va, P.zw);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

    EndPrimitive();
}