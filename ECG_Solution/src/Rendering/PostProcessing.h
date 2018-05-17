#pragma once
#include <GL\glew.h>
#include "../Shader/_Shader.h"
#include <memory>

class PostProcessing
{
private:
	GLuint framebuffer, textureColorbuffer, rbo, quadVAO, quadVBO;
	unsigned int _width, _height;
	std::shared_ptr<_Shader> shader;
public:
	PostProcessing(int width, int height);
	~PostProcessing();
	bool Init();
	void use();
	void draw();
};

