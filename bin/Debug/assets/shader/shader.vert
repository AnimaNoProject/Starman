#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 texture_coord;
} vert;

uniform mat4 model;
uniform mat4 viewProj;
uniform mat3 normals;

void main()
{	
	vec4 position_world = model * vec4(aPos, 1);

	vert.texture_coord = aTexCoords; 
	vert.position_world = position_world.xyz;
	vert.normal_world = normals * aNormal;
	vert.normal_world = normalize(vert.normal_world);

    gl_Position = viewProj * position_world;
}