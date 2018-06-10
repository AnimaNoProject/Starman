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
	GLuint atomicCounter;
	GLuint temp_buffer;
	GLuint vaos[2];
	unsigned int index = 0;
	float particle_count;
	float _maxParticle;

public:
	ParticleSystem(int maxParticle);
	~ParticleSystem();
	void calculate(float deltaTime);
	void draw(mat4 viewProj);
};
