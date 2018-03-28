#include "_Shader.h"

std::string _Shader::readFile(const char *filePath) {
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

_Shader::_Shader(const char *vertex_path, const char *fragment_path)
{
	LoadShader(vertex_path, fragment_path);
}

_Shader::~_Shader()
{
}

unsigned int _Shader::getUniform(std::string uniform)
{
	return glGetUniformLocation(shader, uniform.c_str());
}

void _Shader::LoadShader(const char *vertex_path, const char *fragment_path) {

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	std::string vertShaderStr = readFile(vertex_path);
	std::string fragShaderStr = readFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	GLint success;

	// Compile vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertShader, 1, (const GLchar**)&vertShaderSrc, 0);
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
	glShaderSource(fragShader, 1, (const GLchar**)&fragShaderSrc, 0);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(fragShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", fragShader, InfoLog);
	}

	std::cout << "Linking program" << std::endl;
	shader = glCreateProgram();
	glAttachShader(shader, vertShader);
	glAttachShader(shader, fragShader);

	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (success == 0) {
		GLchar ErrorLog[1024];
		glGetProgramInfoLog(shader, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}

	//glDetachShader(shader, fragmentShader);
	//glDetachShader(shader, vertexShader);
}

GLuint _Shader::getShader()
{
	return shader;
}

void _Shader::use()
{
	glUseProgram(shader);
}

void _Shader::setUniform(std::string uniform, const int i)
{
	glUniform1i(getUniform(uniform), i);
}

void _Shader::setUniform(std::string uniform, const unsigned int i) {
	glUniform1ui(getUniform(uniform), i);
}

void _Shader::setUniform(std::string uniform, const float f) {
	glUniform1f(getUniform(uniform), f);
}

void _Shader::setUniform(std::string uniform, const glm::mat4& mat) {
	glUniformMatrix4fv(getUniform(uniform), 1, GL_FALSE, glm::value_ptr(mat));
}

void _Shader::setUniform(std::string uniform, const glm::mat3& mat) {
	glUniformMatrix3fv(getUniform(uniform), 1, GL_FALSE, glm::value_ptr(mat));
}

void _Shader::setUniform(std::string uniform, const glm::vec2& vec) {
	glUniform2fv(getUniform(uniform), 1, glm::value_ptr(vec));
}

void _Shader::setUniform(std::string uniform, const glm::vec3& vec) {
	glUniform3fv(getUniform(uniform), 1, glm::value_ptr(vec));
}

void _Shader::setUniform(std::string uniform, const glm::vec4& vec) {
	glUniform4fv(getUniform(uniform), 1, glm::value_ptr(vec));
}