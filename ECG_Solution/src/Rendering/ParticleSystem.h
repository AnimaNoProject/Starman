#pragma once

#include "../Shader/_Shader.h"
#include "GL\glew.h"
#include <random>
#include <vector>
#include <ctime>

class ParticleSystem
{
private:
	_Shader * computeShader;
	_Shader * drawShader;
	GLuint ssbo_pos[2];
	GLuint ssbo_vel[2];
	GLuint vaos[2];
	GLuint atomicCounter;
	GLuint temp_buffer;
	unsigned int index = 0;
	GLuint particle_count;
	GLfloat pp;
	GLuint _maxParticle;

public:
	ParticleSystem(int maxParticle);

	~ParticleSystem();
	void calculate(const glm::vec3& pos, const glm::vec3& dir, float deltaTime);
	void draw(const mat4& view, const mat4& proj);
};
