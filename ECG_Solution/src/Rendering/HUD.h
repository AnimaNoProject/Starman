#pragma once

#include <iostream>
#include <map>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Shader/_Shader.h"
struct Character {
	GLuint TextureID;
	glm::ivec2 Size;   
	glm::ivec2 Bearing;  // offset from baseline to top left
	GLuint Advance;  // horizontal offset
};

class HUD
{
private:
	std::map<GLchar, Character> Characters;
	float _height, _width;
	mat4 _projection;
	FT_Library ft;
	FT_Face face;
	FT_GlyphSlot glyph;
	_Shader* _shader;
	GLuint vbo, vao, tex;
public:
	HUD(_Shader* shader, float height, float width);
	~HUD();
	bool initialize();
	void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	void render(float deltaTime, bool debug_mode, int healtpoints, int speed);
	mat4 getProjection();
};