#pragma once
#include <GL\glew.h>
#include "../Shader/_Shader.h"
#include <memory>

class PostProcessing
{
private:
	GLuint vbo_fbo_vertices;
	GLuint framebuffer = 0;
	GLuint renderedTexture;
	int _width, _height;
	GLuint depthbuffer;
	GLenum DrawBuffers[1];
	std::shared_ptr<_Shader> shader;
public:
	PostProcessing(int width, int height);
	~PostProcessing();
	bool Init();
	void use();
	void draw();
};

