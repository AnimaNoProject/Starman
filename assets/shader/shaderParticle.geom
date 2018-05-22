#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
out vec4 color;

in vec4 out_color[];


void main(void)
{
    int i = 0;
    int points = 0;

    for ( i = 0; i < gl_in.length(); i++)
    {
		gl_Position = gl_in[i].gl_Position + vec4(-1, -1, 0, 0);
		color = out_color[i];
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(1, -1, 0, 0);
        color = out_color[i];
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(-1, 1, 0, 0);
        color = out_color[i];
        EmitVertex();

		gl_Position = gl_in[i].gl_Position + vec4(1, 1, 0, 0);
        color = out_color[i];
        EmitVertex();

        EndPrimitive();
    }
}