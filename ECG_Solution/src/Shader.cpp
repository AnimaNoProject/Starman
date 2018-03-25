#include "Shader.h"

std::string Shader::readFile(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}


GLuint Shader::LoadShader(const char *vertex_path, const char *fragment_path) {
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	std::string vertShaderStr = readFile(vertex_path);
	std::string fragShaderStr = readFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	GLint success;
	int logLength;

	// Compile vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	// Check vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(vertexShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", vertexShader, InfoLog);
	}

	// Compile fragment shader
	std::cout << "Compiling fragment shader." << std::endl;
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(fragShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", fragShader, InfoLog);
	}

	std::cout << "Linking program" << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glBindAttribLocation(program, 0, "in_Position");

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == 0) {
		GLchar ErrorLog[1024];
		glGetProgramInfoLog(program, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}