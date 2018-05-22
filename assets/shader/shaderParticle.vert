#version 420

in layout (location = 0) vec4 position; 
in layout (location = 1) vec3 color;

uniform mat4 viewProj;

out vec4 out_color;


void main()
{
	gl_Position = viewProj * vec4(position.xyz, 1.0);
	out_color = vec4(color, 1.0);
}
