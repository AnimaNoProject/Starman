#pragma once

#include "../Shader/_Shader.h"
#include "GL\glew.h"
#include <random>
#include <vector>
#include <ctime>

struct Position
{
	float x;
	float y;
	float z;
	float w;
};

struct Velocity
{
	float vx;
	float vy;
	float vz;
	float ttl;
};

class ParticleSystem
{
public:
	ParticleSystem(int number);
	~ParticleSystem();
	void Init(unsigned int workgroup_x, unsigned int workgroup_y, unsigned int workgroup_z);
	void Update(float d_time);
	void InitRender(const Position* positions, const Velocity* velocities);
	void Draw();
private:
	unsigned int particles_alive;
	unsigned int particles_max;
	int outputIndex;
	unsigned int workGroup[3];

	_Shader* compute_shader;
	GLuint posBuffer[2];
	GLuint velBuffer[2];
	GLuint vao[2];
};
