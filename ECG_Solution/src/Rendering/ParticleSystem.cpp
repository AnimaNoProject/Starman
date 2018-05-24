#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int number)
{
	velBuffer = colBuffer = posBuffer = 0;
}

ParticleSystem::~ParticleSystem()
{
	// delete buffers
	glDeleteBuffers(1, &velBuffer);
	glDeleteBuffers(1, &posBuffer);
	glDeleteBuffers(1, &posBuffer);
}

void ParticleSystem::Init(unsigned int workgroup_x, unsigned int workgroup_y, unsigned int workgroup_z)
{
	workGroup[0] = workgroup_x;
	workGroup[1] = workgroup_y;
	workGroup[2] = workgroup_z;
	shader = new _Shader("assets/shader/shaderParticle.vert", "assets/shader/shaderParticle.frag");
	compute_shader = new _Shader("assets/shader/shaderParticle.comp");

	vec4* points;
	vec4* velocities;
	vec4* colors;

	points = (vec4*)calloc(MAX_PARTICLES, sizeof(vec4));
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		points[i].x = rand() % 100;
		points[i].y = rand() % 100;
		points[i].z = rand() % 100;
		points[i].w = 1;
	}
	glGenBuffers(1, &posBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(vec4), points, GL_DYNAMIC_COPY);

	velocities = (vec4*)calloc(MAX_PARTICLES, sizeof(vec4));
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		velocities[i].x = rand() % 4;
		velocities[i].y = rand() % 4;
		velocities[i].z = rand() % 4;
		velocities[i].w = rand() % 4;
	}
	glGenBuffers(1, &velBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, velBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(vec4), velocities, GL_DYNAMIC_COPY);

	colors = (vec4*)calloc(MAX_PARTICLES, sizeof(vec4));
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		colors[i].x = rand() % 100 / 100;
		colors[i].y = rand() % 100 / 100;
		colors[i].z = rand() % 100 / 100;
		colors[i].w = 1;
	}
	glGenBuffers(1, &colBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(vec4), colors, GL_DYNAMIC_COPY);
}

void ParticleSystem::Update(float d_time)
{
	compute_shader->use();
	glDisable(GL_CULL_FACE);
	glPointSize(4.5f);
	// bind uniforms
	compute_shader->setUniform("d_time", d_time);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, colBuffer);

	glDispatchCompute(MAX_PARTICLES / WORK_GROUP_SIZE, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glUseProgram(0);
}

void ParticleSystem::Draw(mat4 viewproj)
{
	shader->use();
	shader->setUniform("viewProj", viewproj);

	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, colBuffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);

	glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

	glEnable(GL_CULL_FACE);
	glUseProgram(0);
}