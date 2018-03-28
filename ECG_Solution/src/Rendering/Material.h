#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "../Shader/_Shader.h"

/* --------------------------------------------- */
// Base material
/* --------------------------------------------- */

class Material
{
protected:
	std::shared_ptr<_Shader> _shader;

	glm::vec3 _materialCoefficients; // x = ambient, y = diffuse, z = specular
	float _alpha;

public:
	Material(std::shared_ptr<_Shader> shader, glm::vec3 materialCoefficients, float specularCoefficient);
	virtual ~Material();

	_Shader* getShader();
	virtual void setUniforms();
};