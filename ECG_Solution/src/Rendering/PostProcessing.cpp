#include "PostProcessing.h"



PostProcessing::PostProcessing(int width, int height) : _width(width), _height(height)
{
	shader = std::make_shared<_Shader>("assets/shader/shaderPost.vert", "assets/shader/shaderPost.frag");
}

bool PostProcessing::Init()
{
	// Framebuffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Texture to render to
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// DepthBuffer
	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	DrawBuffers[0] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	GLfloat fbo_vertices[] = {
		0, 0,
		1, 0,
		0,  1,
		1,  1,
	};
	glGenBuffers(1, &vbo_fbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void PostProcessing::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, _width, _height);
}

void PostProcessing::draw()
{
	shader.get()->use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _width, _height);
	shader.get()->setUniform("renderedTexture", renderedTexture);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);

	GLuint UV = shader.get()->getUniform("UV");

	glEnableVertexAttribArray(UV);

	glVertexAttribPointer(
		UV,  // attribute
		2,                  // number of elements per vertex, here (x,y)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(UV);
}


PostProcessing::~PostProcessing()
{
}
