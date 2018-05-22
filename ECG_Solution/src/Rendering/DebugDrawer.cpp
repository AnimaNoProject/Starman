#include "DebugDrawer.h"

DebugDrawer::DebugDrawer() : _debugMode(1)
{
}

DebugDrawer::~DebugDrawer()
{
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	LINES.push_back(_LINE(vec3(from.getX(), from.getY(), from.getZ()), vec3(to.getX(), to.getY(), to.getZ())));
	COLORS.push_back(_COLOR(vec3(color.getX(), color.getY(), color.getZ())));

	setupDrawing();
	render();
	cleanDrawing();
}

void DebugDrawer::setupDrawing()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, LINES.size() * sizeof(_LINE), &LINES[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, COLORS.size() * sizeof(_COLOR), &COLORS[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(LINES), 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(COLORS), 0);
	
	glBindVertexArray(0);
}

void DebugDrawer::render()
{
	/*glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, LINES.size() * 2);
	// Clear vectors
	LINES.clear();
	COLORS.clear();*/

	//_shader->use();
	//_shader->setUniform("viewProj", camera.getViewProjectionMatrix());
	vector<GLfloat> vertices;
	vector<GLuint> indices;
	unsigned int indexI = 0;

	for (vector<DebugDrawer::_LINE>::iterator it = LINES.begin(); it != LINES.end(); it++)
	{
		DebugDrawer::_LINE l = *it;

		vertices.push_back(l.from.x);
		vertices.push_back(l.from.y);
		vertices.push_back(l.from.z);

		vertices.push_back(l.to.x);
		vertices.push_back(l.to.y);
		vertices.push_back(l.to.z);

		indices.push_back(indexI);
		indices.push_back(indexI + 1);
		indexI += 2;
	}
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, (void*)&indices[0]);
}

void DebugDrawer::setShader(_Shader* shader)
{
	_shader = shader;
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
}

void DebugDrawer::setDebugMode(int m_debugMode)
{
	_debugMode = m_debugMode;
}

int DebugDrawer::getDebugMode() const
{
	return _debugMode;
}

void DebugDrawer::cleanDrawing()
{
	// delete buffers
	glDeleteBuffers(2, vbo);
	glDeleteBuffers(1, &vao);
}

