#version 430 core

out vec4 color;

in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 texture_coord;
} vert;

uniform sampler2D texture_diffuse1;		
uniform sampler2D texture_specular1;  // Lightmap will be stored as specular texture

uniform float brightness;
uniform vec3 camera_world;

void main()
{    
	vec3 base_texture = texture2D(texture_diffuse1, vert.texture_coord).rgb;
	vec3 lightmap = texture2D(texture_specular1, vert.texture_coord).rgb;
	vec3 model_texture = base_texture * lightmap;
	color = vec4(model_texture * brightness, 1);
}
















