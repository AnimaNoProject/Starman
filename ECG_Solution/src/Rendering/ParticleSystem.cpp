#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int number) : outputIndex(1)
{
	particles_max = number;
	velBuffer[0] = velBuffer[1] = 0;
	posBuffer[0] = posBuffer[1] = 0;
}

ParticleSystem::~ParticleSystem()
{
	// delete buffers
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(1, &velBuffer[0]);
	glDeleteBuffers(1, &velBuffer[1]);
	glDeleteBuffers(1, &posBuffer[0]);
	glDeleteBuffers(1, &posBuffer[1]);
}

void ParticleSystem::Init(unsigned int workgroup_x, unsigned int workgroup_y, unsigned int workgroup_z)
{
	workGroup[0] = workgroup_x;
	workGroup[1] = workgroup_y;
	workGroup[2] = workgroup_z;

	particles_alive = particles_max;

	Position* positions = new Position[particles_max];
	Velocity* velocities = new Velocity[particles_max];

	memset(velocities, 0, particles_max * sizeof(Velocity));

	Position* pPositions = positions;
	Velocity* pSpeed = velocities;

	for (unsigned int i = 0; i < particles_alive; i++)
	{
		// Position + Color bit
		pPositions->x = 0.0f;
		pPositions->y = 0.0f;
		pPositions->z = 0.0f;
		pPositions->w = 0.8;

		// Speed + TimeToLive
		pSpeed->vx = rand() % 5;
		pSpeed->vz = rand() % 5;
		pSpeed->vy = rand() % 5;
		pSpeed->ttl = 10.0f;
	}

	InitRender(positions, velocities);

	delete[] positions;
	delete[] velocities;
}

void ParticleSystem::Update(float d_time)
{
	compute_shader->use();

	// bind uniforms
	compute_shader->setUniform("d_time", d_time);
	compute_shader->setUniform("particles_alive", particles_alive);

	// bind buffers
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, posBuffer[!outputIndex], 0, particles_alive * sizeof(Position));
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, velBuffer[!outputIndex], 0, particles_alive * sizeof(Velocity));

	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 2, posBuffer[outputIndex], 0, particles_alive * sizeof(Position));
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, velBuffer[outputIndex], 0, particles_alive * sizeof(Velocity));

	//execute the compute shader
	glDispatchCompute(workGroup[0], workGroup[1], workGroup[2]);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);

	//Swap input and output
	outputIndex = !outputIndex;
}

void ParticleSystem::InitRender(const Position * positions, const Velocity * velocities)
{
	compute_shader = new _Shader("assets/shader/shaderParticle.comp");

	// Generate Position / Velocity Buffers
	glGenBuffers(1, &posBuffer[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posBuffer[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particles_alive * sizeof(positions), (float*)positions, GL_STATIC_DRAW);
	
	glGenBuffers(1, &posBuffer[1]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posBuffer[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particles_alive * sizeof(positions), (float*)positions, GL_STATIC_DRAW);

	glGenBuffers(1, &velBuffer[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBuffer[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particles_alive * sizeof(velocities), (float*)velocities, GL_STATIC_DRAW);

	glGenBuffers(1, &velBuffer[1]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBuffer[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particles_alive*sizeof(velocities), (float*)velocities, GL_STATIC_DRAW);
	//

	// VAOs
	glGenVertexArrays(2, vao);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer[0]);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer[1]);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	//
}

void ParticleSystem::Draw()
{
	// draw vaos
	glBindVertexArray(vao[outputIndex]);
	glDrawArrays(GL_POINTS, 0, particles_alive);
	glBindVertexArray(0);
	glUseProgram(0);
}