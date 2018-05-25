#pragma once

#include <glm\glm.hpp>

struct DirectionalLight {
	DirectionalLight() {
		enabled = false;
	}

	DirectionalLight(glm::vec3 color, glm::vec3 direction, bool enabled = true)
		: color(color), direction(glm::normalize(direction)), enabled(enabled){}

	bool enabled;
	glm::vec3 color;
	glm::vec3 direction;
};

struct PointLight {
	PointLight() {
		enabled = false;
	}

	PointLight(glm::vec3 color, glm::vec3 position, glm::vec3 attenuation, bool enabled= true)
		: color(color), position(position), attenuation(attenuation), enabled(enabled){}

	bool enabled;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 attenuation; // x = constant, y = linear, z = quadratic 
};

struct SpotLight {
	SpotLight()
	{
		enabled = false;
	}
	SpotLight(vec3 color, vec3 position, vec3 attentuation, vec3 coneDirection, float coneAngle, bool enabled= true) : color(color), attentuation(attentuation), coneAngle(coneAngle), coneDirection(coneDirection), enabled(enabled) {}
	void setPosition(vec3 pos) { position = pos; }
	void setDirection(vec3 dir) { coneDirection = dir; }
	vec3 position;
	bool enabled;
	vec3 color;
	vec3 attentuation;
	float coneAngle;
	vec3 coneDirection;
};
