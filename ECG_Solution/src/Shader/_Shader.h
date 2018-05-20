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

using namespace glm;
using namespace std;

class _Shader
{
protected:
	GLuint vertexShader, fragmentShader, geometryShader, shader;
	const GLchar* vertex_shader_source;
	const GLchar* fragment_shader_source;
	GLchar* geometry_shader_source;
	void LoadShader(const char *vertex_path, const char *fragment_path);
	void LoadShader(const char *vertex_path, const char *fragment_path, const char *geometry_path);
public:
	_Shader(const char *vertex_path, const char *fragment_path);
	_Shader(const char *vertex_path, const char *fragment_path, const char *geometry_path);
	~_Shader();
	string readFile(const char *filePath);
	GLuint getShader();
	void use();
	unsigned int getUniform(std::string uniform);
	void setUniform(std::string uniform, const int i);
	void setUniform(std::string uniform, const unsigned int i);
	void setUniform(std::string uniform, const float f);
	void setUniform(std::string uniform, const mat4& mat);
	void setUniform(std::string uniform, const mat3& mat);
	void setUniform(std::string uniform, const vec2& vec);
	void setUniform(std::string uniform, const vec3& vec);
	void setUniform(std::string uniform, const vec4& vec);
};