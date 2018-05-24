#include "DebugDrawer.h"

DebugDrawer::DebugDrawer() : m_debugMode(1)
{
}

DebugDrawer::~DebugDrawer(void)
{
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	/*
	vec3 src(from.x(), from.y(), from.z());
	vec3 dst(to.x(), to.y(), to.z());
	vec3 col(color.x(), color.y(), color.z());

	LINE l(src, dst, col);

	//vec3 _from(from.x(), from.y(), from.z());
	//vec3 _to(to.x(), to.y(), to.z());

	//LINE line;
	//line.a = _from;
	//line.b = _to;
	lines.push_back(l);
	//lines.push_back(line);
	*/
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

	//setupDrawing();
	//render();
	//cleanDrawing();
}

void DebugDrawer::setSader(_Shader* shader)
{
	_shader = shader;
}

void DebugDrawer::setViewProj(mat4 matrix)
{
	viewProj = matrix;
}


/*
void DebugDrawer::setupDrawing()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(LINE), &lines[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(lines), (void*)0);
	
	glBindVertexArray(0);
}

void DebugDrawer::render()
{
	vector<GLfloat> vertices;
	vector<GLuint> indices;
	unsigned int indexI = 0;

	for (vector<DebugDrawer::LINE>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		DebugDrawer::LINE l = *it;

		vertices.push_back(l.a.x);
		vertices.push_back(l.a.y);
		vertices.push_back(l.a.z);

		vertices.push_back(l.b.x);
		vertices.push_back(l.b.y);
		vertices.push_back(l.b.z);

		indices.push_back(indexI);
		indices.push_back(indexI + 1);
		indexI += 2;
	}
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, (void*)&indices[0]);
	glBindVertexArray(0);
}

void DebugDrawer::cleanDrawing()
{
	// delete buffers
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vao);
}

*/

