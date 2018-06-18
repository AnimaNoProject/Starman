#include "ParticleSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include "ProceduralTexture.h"

ParticleSystem::ParticleSystem(int maxParticle) : particles_to_spawn(0)
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
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &value);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	glGenBuffers(1, &temp_buffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp_buffer);
	glBufferData(GL_COPY_WRITE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_READ);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

	const int TTL = 5;
	std::vector<glm::vec4> positions;
	std::vector<glm::vec4> velocities;

	positions.push_back(vec4(-5000, 0, 0, TTL));
	positions.push_back(vec4(-5000, 0, 0, TTL));
	velocities.push_back(vec4(0, 0, 10, 0));
	velocities.push_back(vec4(0, 0, 10, 0));

	particle_count = positions.size();
	//pp = static_cast<float>(particle_count);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particle_count * sizeof(glm::vec4), positions.data());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particle_count * sizeof(glm::vec4), velocities.data());


	glGenVertexArrays(1, &vaos[0]);
	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo_pos[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vaos[1]);
	glBindVertexArray(vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo_pos[1]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
	glBindVertexArray(0);


	// Load texture
	int height(420);
	int numComponents(4);
	unsigned char *data = stbi_load("assets/objects/explosion.png", &height, &height, &numComponents, STBI_rgb_alpha);

	// Bind Texture and set texture parameters
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 420, 420, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

ParticleSystem::~ParticleSystem()
{
	delete computeShader;
	delete drawShader;
	glDeleteBuffers(2, ssbo_pos);
	glDeleteBuffers(2, ssbo_vel);
	glDeleteVertexArrays(2, vaos);
}

void ParticleSystem::calculate(const glm::vec3& pos, const glm::vec3& dir, float deltaTime)
{
	const double spawnRatePerSecond = 5;
	particles_to_spawn += (spawnRatePerSecond * deltaTime);

	GLuint spawnCount = 0;
	if (particles_to_spawn > 0)
	{
		spawnCount += (GLuint)particles_to_spawn;
		particles_to_spawn -= spawnCount;
	}

	computeShader->use();
	computeShader->setUniform("DeltaT", deltaTime);
	computeShader->setUniform("LastCount", particle_count);
	computeShader->setUniform("MaximumCount", _maxParticle);
	computeShader->setUniform("P", pos);
	computeShader->setUniform("Dir", dir);
	computeShader->setUniform("SpawnCount", spawnCount);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_pos[index]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo_vel[index]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo_pos[!index]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_vel[!index]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, atomicCounter);

	index = !index;
	GLuint groups = (particle_count / (16 * 16)) + 1;
	glDispatchCompute(groups, 1, 1);

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicCounter);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp_buffer);
	
	glCopyBufferSubData(GL_ATOMIC_COUNTER_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(GLuint));

	GLuint *counterValue = (GLuint*)glMapBufferRange(GL_COPY_WRITE_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	particle_count = counterValue[0];
	counterValue[0] = 0;
	glUnmapBuffer(GL_COPY_WRITE_BUFFER);
	glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ATOMIC_COUNTER_BUFFER, 0, 0, sizeof(GLuint));

	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void ParticleSystem::draw(const mat4& view, const mat4& proj)
{
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_MAX);

	drawShader->use();
	drawShader->setUniform("view", view);
	drawShader->setUniform("proj", proj);
	drawShader->setUniform("model", mat4(1));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(vaos[index]);
	glDrawArraysInstanced(GL_POINTS, 0, particle_count * sizeof(unsigned int), particle_count);
	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
