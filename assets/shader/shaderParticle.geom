#version 430 core

layout (points) in;
layout(triangle_strip, max_vertices=4) out;

uniform mat4 proj;

in vData { 
	float TTL1; 
} vertex[];

out vec2 TexCoord0;
flat out float TTL0;

void main(void)
{
	float scale = 0.5;
	vec4 position = gl_in[0].gl_Position;

	gl_Position =  proj * (position + vec4(-1, -1, 0, 0) * scale);
	TexCoord0 = vec2(0.0, 0.0);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

	gl_Position =  proj * (position + vec4(1, -1, 0, 0) * scale);
	TexCoord0 = vec2(1.0, 0.0);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

	gl_Position =  proj * (position + vec4(-1, 1, 0, 0) * scale);
	TexCoord0 = vec2(0.0, 1.0);
	TTL0 = vertex[0].TTL1;
	EmitVertex();
	
	gl_Position =  proj * (position + vec4(1, 1, 0, 0) * scale);
	TexCoord0 = vec2(1.0, 1.0);
	TTL0 = vertex[0].TTL1;
	EmitVertex();

    EndPrimitive();
}