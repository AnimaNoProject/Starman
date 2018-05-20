#include "_Shader.h"

string _Shader::readFile(const char *filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);

	if (!fileStream.is_open()) {
		cerr << "Could not read file " << filePath << ". File does not exist." << endl;
		return "";
	}

	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");

	}
	fileStream.close();
	return content;
}

_Shader::_Shader(const char * compute_path)
{
	LoadShader(compute_path);
}

_Shader::_Shader(const char *vertex_path, const char *fragment_path)
{
	LoadShader(vertex_path, fragment_path);
}

_Shader::_Shader(const char * vertex_path, const char * fragment_path, const char * geometry_path)
{
	LoadShader(vertex_path, fragment_path, geometry_path);
}

_Shader::~_Shader()
{
}

unsigned int _Shader::getUniform(string uniform)
{
	return glGetUniformLocation(shader, uniform.c_str());
}

void _Shader::LoadShader(const char * compute_path)
{
	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);

	string computeShaderStr = readFile(compute_path);
	const char *computeShaderSrc = computeShaderStr.c_str();

	GLint success;

	// Compile compute shader
	cout << "Compiling vertex shader." << endl;
	glShaderSource(computeShader, 1, (const GLchar**)&computeShaderSrc, 0);
	glCompileShader(computeShader);

	// Check compute shader
	glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(computeShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", computeShader, InfoLog);
	}

	cout << "Linking program" << endl;
	shader = glCreateProgram();
	glAttachShader(shader, computeShader);

	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (success == 0) {
		GLchar ErrorLog[1024];
		glGetProgramInfoLog(shader, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}
}

void _Shader::LoadShader(const char *vertex_path, const char *fragment_path) {

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	string vertShaderStr = readFile(vertex_path);
	string fragShaderStr = readFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	GLint success;

	// Compile vertex shader
	cout << "Compiling vertex shader." << endl;
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
	cout << "Compiling fragment shader." << endl;
	glShaderSource(fragShader, 1, (const GLchar**)&fragShaderSrc, 0);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(fragShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", fragShader, InfoLog);
	}

	cout << "Linking program" << endl;
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
}

void _Shader::LoadShader(const char * vertex_path, const char * fragment_path, const char * geometry_path)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint geomShader = glCreateShader(GL_GEOMETRY_SHADER);

	// Read shaders
	string vertShaderStr = readFile(vertex_path);
	string fragShaderStr = readFile(fragment_path);
	string geomShaderStr = readFile(geometry_path);

	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();
	const char *geomShaderSrc = geomShaderStr.c_str();

	GLint success;

	// Compile vertex shader
	cout << "Compiling vertex shader." << endl;
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
	cout << "Compiling fragment shader." << endl;
	glShaderSource(fragShader, 1, (const GLchar**)&fragShaderSrc, 0);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(fragShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", fragShader, InfoLog);
	}

	// Compile geoemtry shader
	cout << "Compiling fragment shader." << endl;
	glShaderSource(geomShader, 1, (const GLchar**)&geomShaderSrc, 0);
	glCompileShader(geomShader);

	// Check geometry shader
	glGetShaderiv(geomShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(geomShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", geomShader, InfoLog);
	}

	cout << "Linking program" << endl;
	shader = glCreateProgram();
	glAttachShader(shader, vertShader);
	glAttachShader(shader, fragShader);
	glAttachShader(shader, geomShader);

	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (success == 0) {
		GLchar ErrorLog[1024];
		glGetProgramInfoLog(shader, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
	}
}

GLuint _Shader::getShader()
{
	return shader;
}

void _Shader::use()
{
	glUseProgram(shader);
}

void _Shader::setUniform(string uniform, const int i)
{
	glUniform1i(getUniform(uniform), i);
}

void _Shader::setUniform(string uniform, const unsigned int i) {
	glUniform1ui(getUniform(uniform), i);
}

void _Shader::setUniform(string uniform, const float f) {
	glUniform1f(getUniform(uniform), f);
}

void _Shader::setUniform(string uniform, const mat4& mat) {
	glUniformMatrix4fv(getUniform(uniform), 1, GL_FALSE, value_ptr(mat));
}

void _Shader::setUniform(string uniform, const mat3& mat) {
	glUniformMatrix3fv(getUniform(uniform), 1, GL_FALSE, value_ptr(mat));
}

void _Shader::setUniform(string uniform, const vec2& vec) {
	glUniform2fv(getUniform(uniform), 1, value_ptr(vec));
}

void _Shader::setUniform(string uniform, const vec3& vec) {
	glUniform3fv(getUniform(uniform), 1, value_ptr(vec));
}

void _Shader::setUniform(string uniform, const vec4& vec) {
	glUniform4fv(getUniform(uniform), 1, value_ptr(vec));
}