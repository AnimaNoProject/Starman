#pragma once
#include <GL\glew.h>
#include "../Shader/_Shader.h"
#include <memory>

class PostProcessing
{
private:
	GLuint framebuffer, textureColor, rbo, quadVAO, quadVBO, textureDepth;
	unsigned int _width, _height;
	float _near, _far;
	std::shared_ptr<_Shader> shader;
public:
	PostProcessing(int width, int height, float near, float far);
	~PostProcessing();
	bool Init();
	void use();
	void draw();
};