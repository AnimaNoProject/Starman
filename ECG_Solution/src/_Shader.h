#pragma once

#include "GL\glew.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class _Shader
{
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
};