#include "DebugDrawer.h"

DebugDrawer::DebugDrawer() : m_debugMode(1)
{
}

DebugDrawer::~DebugDrawer(void)
{
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	btLine lines(from, to);

	btVector3 result = to - from;
	glm::vec3 colors = { result.x(), result.y(), result.z() };
	colors = glm::normalize(colors);

	GLuint indices[] = { 0,1 };

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	//UPLOADING VERTEX
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6, lines.vertices, GL_STATIC_DRAW);
	//UPLOADING INDEXES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 2, indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0);
	glBindVertexArray(0);

	_shader->use();
	_shader->setUniform("viewProj", viewProj);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//delete buffers
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

}

void DebugDrawer::setSader(_Shader* shader)
{
	_shader = shader;
}

void DebugDrawer::setViewProj(mat4 matrix)
{
	viewProj = matrix;
}