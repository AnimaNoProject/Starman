#version 430 core

layout (points) in;
layout(points, max_vertices=4) out;

void main(void)
{
    for (int i = 0; i < gl_in.length(); i++)
    {
		gl_Position = gl_in[i].gl_Position + vec4(0.0, 0.0, 0.0, 0.0);
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(0.0, 5.0, 0.0, 0.0);
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(5.0, 0.0, 0.0, 0.0);
        EmitVertex();

		gl_Position = gl_in[i].gl_Position + vec4(5.0, 5.0, 0.0, 0.0);
        EmitVertex();

        EndPrimitive();
}
}