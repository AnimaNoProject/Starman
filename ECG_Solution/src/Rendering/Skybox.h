#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>

#include "stb_image.h"
#include <iostream>
#include "../Shader/_Shader.h"

using namespace glm;

class Skybox
{
private:
	_Shader* skybox_shader;
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int texture;
	unsigned int loadCubemap(std::vector < std::string > faces);
public:
	Skybox();
	~Skybox();
	void Draw(mat4 view, mat4 proj);
};

