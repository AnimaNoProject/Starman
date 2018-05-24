#pragma once
#include <LinearMath/btIDebugDraw.h>
#include "../Shader/_Shader.h"
#include "../GameObjects/Camera.h"
#include "../GameObjects/Camera.h"

using namespace glm;

class DebugDrawer : public btIDebugDraw
{

public:

	struct btLine {
		GLfloat vertices[6];
		btLine(const btVector3& f, const btVector3& t) {
			vertices[0] = f.x();
			vertices[1] = f.y();
			vertices[2] = f.z();

			vertices[3] = t.x();
			vertices[4] = t.y();
			vertices[5] = t.z();
		}
	};

	DebugDrawer();
	~DebugDrawer(void);

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {}
	virtual void drawTriangle(const btVector3 & a, const btVector3 & b, const btVector3 & c, const btVector3 & color, btScalar alpha) {}

	void reportErrorWarning(const char * warningString) { std::cout << "Physics debugger warning: " << warningString << std::endl; }

	virtual void draw3dText(const btVector3& location, const char* textString) {}
	virtual void setDebugMode(int debugMode) { m_debugMode = debugMode; }
	virtual int getDebugMode() const { return m_debugMode; }

	void setViewProj(mat4 matrix);

	void setSader(_Shader* shader);

private:
	//std::vector<LINE> lines;

	int m_debugMode;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	_Shader* _shader;
	mat4 viewProj;
};
