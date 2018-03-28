#pragma once

#include "GL\glew.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class _Shader
{
private:
	unsigned int getUniform(std::string uniform);

protected:
	GLuint vertexShader, fragmentShader, shader;
	const GLchar* vertex_shader_source;
	const GLchar* fragment_shader_source;
	void LoadShader(const char *vertex_path, const char *fragment_path);

public:
	_Shader(const char *vertex_path, const char *fragment_path);
	~_Shader();
	std::string readFile(const char *filePath);
	GLuint getShader();
	void use();
	void setUniform(std::string uniform, const int i);
	void setUniform(std::string uniform, const unsigned int i);
	void setUniform(std::string uniform, const float f);
	void setUniform(std::string uniform, const glm::mat4& mat);
	void setUniform(std::string uniform, const glm::mat3& mat);
	void setUniform(std::string uniform, const glm::vec2& vec);
	void setUniform(std::string uniform, const glm::vec3& vec);
	void setUniform(std::string uniform, const glm::vec4& vec);
};