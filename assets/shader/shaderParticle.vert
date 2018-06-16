#version 430

in layout (location = 0) vec4 position; 

uniform mat4 view;

out vData
{
	float TTL1;
} vertex;

//flat in vec4 pos;

void main()
{
	/*vec4 translPos = pos;
	mat4 transl;
	mat4[0][0] = 1;
	mat4[1][1] = 1;
	mat4[2][2] = 1;
	mat4[3][3] = 1;
	mat4[3][0] = translPos.x;
	mat4[3][1] = translPos.y;
	mat4[3][2] = translPos.z;
	gl_Position = view * transl * vec4(position.xyz, 1.0);*/
	gl_Position = view * mat4(1) * vec4(position.xyz, 1.0);
	vertex.TTL1 = position.w;
}