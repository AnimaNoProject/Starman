#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int number)
{
	velBuffer = lifeBuffer = posBuffer = 0;
}

ParticleSystem::~ParticleSystem()
{
	// delete buffers
	glDeleteBuffers(1, &velBuffer);
	glDeleteBuffers(1, &posBuffer);
	glDeleteBuffers(1, &lifeBuffer);
}

void ParticleSystem::Init(unsigned int workgroup_x, unsigned int workgroup_y, unsigned int workgroup_z)
{
	workGroup[0] = workgroup_x;
	workGroup[1] = workgroup_y;
	workGroup[2] = workgroup_z;
	shader = new _Shader("assets/shader/shaderParticle.vert", "assets/shader/shaderParticle.frag");
	compute_shader = new _Shader("assets/shader/shaderParticle.comp");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &ibo);


	points = (vec4*)calloc(MAX_PARTICLES, sizeof(vec4));
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		points[i].x = rand() % 50;
		points[i].y = rand() % 50;
		points[i].z = rand() % 50;
		points[i].w = 1.0f;
	}

	glGenBuffers(1, &posBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(vec4), points, GL_DYNAMIC_COPY);




	velocities = (vec4*)calloc(MAX_PARTICLES, sizeof(vec4));
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		velocities[i].x = rand() % 4;
		velocities[i].y = rand() % 4;
		velocities[i].z = rand() % 4;
		velocities[i].w = rand() % 4;
	}
	glGenBuffers(1, &velBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(vec4), velocities, GL_DYNAMIC_COPY);

	life = (vec4*)calloc(MAX_PARTICLES, sizeof(vec4));
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		life[i].x = 5.0f;
	}

	glGenBuffers(1, &lifeBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, lifeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(float), life, GL_DYNAMIC_COPY);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, lifeBuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), 0);

	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), 0);

	glBindVertexArray(0);
}

void ParticleSystem::Update(float d_time)
{
	compute_shader->use();
	glDisable(GL_CULL_FACE);

	// bind uniforms
	compute_shader->setUniform("d_time", d_time);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, lifeBuffer);

	glDispatchCompute(MAX_PARTICLES / WORK_GROUP_SIZE, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glUseProgram(0);
}

void ParticleSystem::Draw(mat4 viewproj)
{
	glBindVertexArray(vao);
	shader->use();
	shader->setUniform("viewProj", viewproj);
	glPointSize(2.0f);
	//glDisable(GL_CULL_FACE);


	glDrawElements(GL_POINTS, MAX_PARTICLES, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}