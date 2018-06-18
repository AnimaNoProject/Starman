#version 430 core

flat in float TTL0;
in vec2 TexCoord0;

uniform sampler2D tex0;

out vec4 FragColor;

void main()
{
	vec2 uv = TexCoord0.xy;

	vec4 texColor = texture(tex0, uv);
	FragColor = texColor;
}	