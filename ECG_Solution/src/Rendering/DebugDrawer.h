#pragma once
#include <LinearMath/btIDebugDraw.h>
#include "../Shader/_Shader.h"
#include "../GameObjects/Camera.h"

using namespace glm;


class DebugDrawer : public btIDebugDraw
{

private:
	int _debugMode;
	_Shader* _shader;

public:
	GLuint vao, vbo[2];


	DebugDrawer(void);
	virtual ~DebugDrawer(void);

	struct _LINE {
		vec3 from;
		vec3 to;

		_LINE(vec3 f, vec3 t) {
			from = f;
			to = t;
		}
	};

	vector<_LINE> LINES;

	struct _COLOR {
		vec3 col;

		_COLOR(vec3 c) {
			col = c;
		}
	};

	vector<_COLOR> COLORS;

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	virtual void reportErrorWarning(const char* warningString);
	virtual void draw3dText(const btVector3& location, const char* textString);
	virtual void setDebugMode(int m_debugMode);
	virtual int getDebugMode() const;

	void setupDrawing();
	void cleanDrawing();
	void setShader(_Shader* shader);
	void render();
};
