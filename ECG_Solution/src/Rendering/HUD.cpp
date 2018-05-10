#include "HUD.h"
#include <GL\glew.h> 

HUD::HUD(_Shader* shader, float height, float width)
{
	_shader = shader;
	_height = height;
	_width = width;
	_projection = ortho(0.0f, static_cast<GLfloat>(_width), 0.0f, static_cast<GLfloat>(_height));
}



HUD::~HUD()
{

}

bool HUD::initialize()
{
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR: Initialization of Freetype failed" << std::endl;
		return false;
	}

	if (FT_New_Face(ft, "assets/fonts/CaviarDreams.ttf", 0, &face))
	{
		std::cout << "ERROR: Loading of Font failed" << std::endl;
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load chars
	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR: Glyph loading failed" << std::endl;
			continue;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Store characters
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy Freetype
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_shader->use();
	_shader->setUniform("projection", _projection);
	_shader->setUniform("textColor", vec4(1.0f, 1.0f, 1.0f, 0.4f));

	// initialization success!
	return true;
}

void HUD::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Iterate through text
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		// Update VBO for each character
		GLfloat vertices[6][4] = {
		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}
}

void HUD::render(float deltaTime, bool debug_mode, unsigned int healtpoints, unsigned int speed, long triangles)
{
	char buffer[1000];
	_shader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	sprintf(buffer, "Health: %i", healtpoints);
	renderText(buffer, 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	sprintf(buffer, "Speed: %i", speed);
	renderText(buffer, 25.0f, 75.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	if (debug_mode)
	{
		sprintf(buffer, "Frametime: %f", deltaTime);
		renderText(buffer, 300.0f, 50.0f, 0.5f, glm::vec3(0.5f, 0.5f, 0.5f));
		sprintf(buffer, "FPS: %.0f", 1 / deltaTime);
		renderText(buffer, 600.0f, 50.0f, 0.5f, glm::vec3(0.5f, 0.5f, 0.5f));
		sprintf(buffer, "Rendered: %i Triangles", triangles);
		renderText(buffer, 300.0f, 25.0f, 0.5f, glm::vec3(0.5f, 0.5f, 0.5f));
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}