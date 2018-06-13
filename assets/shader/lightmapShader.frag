#version 430 core

out vec4 color;

in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 texture_coord;
} vert;

uniform sampler2D texture_diffuse1;		
uniform sampler2D texture_specular1;  // Lightmap will be stored as diffuse texture with number 2

uniform float brightness;
uniform vec3 camera_world;

uniform bool cellshading;

uniform float Ka;
uniform float Kd;
uniform float Ks;
uniform float shyniness;

uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} sun;

struct PointLight {
	vec3 color;
	vec3 position;
	vec3 attenuation;
};

struct SpotLight {
	vec3 position;
	bool enabled;
	vec3 color;
	vec3 attentuation;
	float coneAngle;
	vec3 coneDirection;
};

uniform float MAX_LIGHTS;
uniform PointLight[20] lights;

const float levels = 4.0;

vec3 phong(vec3 n, vec3 l, vec3 v, vec3 diffuseC, float diffuseF, vec3 specularC, float specularF, float alpha, bool attenuate, vec3 attenuation) {
	float d = length(l);
	l = normalize(l);
	float att = 1.0;	
	if(attenuate) 
		att = 1.0f / (attenuation.x + d * attenuation.y + d * d * attenuation.z);
	float intensity = max(0, dot(n, l));
	if(cellshading)
	{
		float level = floor(intensity * levels);
		intensity = level / levels;
	}
	vec3 r = reflect(-l, n);
	float intensity_spec = pow(max(0, dot(r, v)), alpha);
	if(cellshading)
	{
		float level = floor(intensity_spec * levels);
		intensity_spec = level / levels;
	}
	return (diffuseF * diffuseC * intensity + specularF * specularC * intensity_spec) * att; 
}

void main()
{    
	vec3 n = normalize(vert.normal_world);
	vec3 v = normalize(camera_world - vert.position_world);
	
	
	vec3 base_texture = texture2D(texture_diffuse1, vert.texture_coord).rgb;
	vec3 lightmap = texture2D(texture_specular1, vert.texture_coord).rgb;
	
	vec3 model_texture = base_texture * lightmap;
	
	color = vec4(model_texture * Ka * brightness, 1); // ambient
	
	// sun
	color.rgb += phong(n, -sun.direction, v, sun.color * model_texture * brightness, Kd, sun.color, Ks, shyniness, false, vec3(0));
}
















