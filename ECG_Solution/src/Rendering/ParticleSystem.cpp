#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int maxParticle)
{
	computeShader = new _Shader("assets/shader/shaderParticle.comp");
	drawShader = new _Shader("assets/shader/shaderParticle.vert", "assets/shader/shaderParticle.frag", "assets/shader/shaderParticle.geom");
	_maxParticle = maxParticle;
	glGenBuffers(1, &ssbo_pos[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, _maxParticle * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ssbo_vel[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, _maxParticle * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ssbo_pos[1]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, _maxParticle * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ssbo_vel[1]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, _maxParticle * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &atomicCounter);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicCounter);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

	GLuint value = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0,sizeof(GLuint), &value);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	glGenBuffers(1, &temp_buffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp_buffer);
	glBufferData(GL_COPY_WRITE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_READ);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

	const int TTL = 1000;
	std::vector<glm::vec4> positions;
	std::vector<glm::vec4> velocities;

	positions.push_back(vec4(0, 0, 0, TTL));
	positions.push_back(vec4(2, 0, 1, TTL));
	velocities.push_back(vec4(0, 1, 0, 0));
	velocities.push_back(vec4(0, 1, 0, 0));

	particle_count = positions.size();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particle_count * sizeof(positions[0]), &positions[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particle_count * sizeof(velocities[0]), &velocities[0]);

	glGenVertexArrays(2, vaos);

	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo_pos[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	glBindVertexArray(vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo_pos[1]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::calculate(float deltaTime)
{
	computeShader->use();
	computeShader->setUniform("DeltaT", deltaTime);
	computeShader->setUniform("LastCount", particle_count);
	computeShader->setUniform("MaximumCount", 1000u);

	const double spawnRatePerSecond = 2;

	particles_to_spawn += spawnRatePerSecond * deltaTime;
	GLuint spawnCount = 0;

	if (particles_to_spawn > 0)
	{
		spawnCount += (GLuint)particles_to_spawn;
		particles_to_spawn -= spawnCount;
	}

	computeShader->setUniform("spawnCount", (GLuint)spawnRatePerSecond);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_pos[index]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo_vel[index]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo_pos[!index]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_vel[!index]);
	
	index = !index;
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, atomicCounter);

	GLuint groups = (particle_count / (16 * 16)) + 1;
	glDispatchCompute(groups, 1, 1);

	glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicCounter);

	GLuint *counterValue = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	particle_count = counterValue[0];

	counterValue[0] = 0;

	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void ParticleSystem::draw(mat4 view, mat4 proj)
{
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_COLOR, GL_SRC_COLOR);
	glDisable(GL_CULL_FACE);
	glBlendEquation(GL_MAX);

	drawShader->use();

	drawShader->setUniform("view", view);
	drawShader->setUniform("proj", proj);

	glBindVertexArray(vaos[index]);
	glDrawArrays(GL_POINTS, 0, particle_count);
	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
}
