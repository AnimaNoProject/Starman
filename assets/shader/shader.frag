#version 430 core

out vec4 color;

in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 texture_coord;
} vert;

uniform sampler2D texture_diff;
uniform float brightness;
uniform vec3 camera_world;

uniform float Ka;
uniform float Kd;
uniform float Ks;
uniform float shinyness;

uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} dirL;

uniform struct PointLight {
	vec3 color;
	vec3 position;
	vec3 attenuation;
} pointL;

vec3 phong(vec3 n, vec3 l, vec3 v, vec3 diffuseC, float diffuseF, vec3 specularC, float specularF, float alpha, bool attenuate, vec3 attenuation) {
	float d = length(l);
	l = normalize(l);
	float att = 1.0;	
	if(attenuate) att = 1.0f / (attenuation.x + d * attenuation.y + d * d * attenuation.z);
	vec3 r = reflect(-l, n);
	return (diffuseF * diffuseC * max(0, dot(n, l)) + specularF * specularC * pow(max(0, dot(r, v)), alpha)) * att; 
}

void main()
{    
	vec3 n = normalize(vert.normal_world);
	vec3 v = normalize(camera_world - vert.position_world);
	
	vec3 texColor = texture(texture_diff, vert.texture_coord).rgb;
	color = vec4(texColor * Ka * brightness, 1); // ambient
	
	// add directional light contribution
	color.rgb += phong(n, -dirL.direction, v, dirL.color * texColor * brightness, Kd, dirL.color, Ks, shinyness, false, vec3(0));
	
	// add point light contribution
	//color.rgb += phong(n, pointL.position - vert.position_world, v, pointL.color * texColor * brightness, materialCoefficients.y, pointL.color, materialCoefficients.z, specularAlpha, true, pointL.attenuation);
}
















