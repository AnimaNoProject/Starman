#pragma once

#include "../Shader/_Shader.h"
#include "GL\glew.h"
#include <random>
#include <vector>
#include <ctime>

#define MAX_PARTICLES 1024 * 1024
#define WORK_GROUP_SIZE 128

struct Position
{
	float x, y, z, w;
};

struct Velocity
{
	float vx, vy, vz, vw;
};

struct Color
{
	float r, g, b, a;
};

class ParticleSystem
{
public:
	ParticleSystem(int number);
	~ParticleSystem();
	void Init(unsigned int workgroup_x, unsigned int workgroup_y, unsigned int workgroup_z);
	void Update(float d_time);
	void Draw(mat4 viewproj);
private:
	unsigned int workGroup[3];
	_Shader* shader;
	_Shader* compute_shader;
	vec4* points;
	vec4* velocities;
	vec4* life;
	GLuint posBuffer, velBuffer, lifeBuffer, vao, ibo;
};
