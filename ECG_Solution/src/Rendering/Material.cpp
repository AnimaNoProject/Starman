#include "Material.h"

/* --------------------------------------------- */
// Base material
/* --------------------------------------------- */

Material::Material(std::shared_ptr<_Shader> shader, glm::vec3 materialCoefficients, float specularCoefficient)
	: _shader(shader), _materialCoefficients(materialCoefficients), _alpha(specularCoefficient)
{
}

Material::~Material()
{
}

_Shader* Material::getShader()
{
	return _shader.get();
}

void Material::setUniforms()
{
	_shader->setUniform("materialCoefficients", _materialCoefficients);
	_shader->setUniform("specularAlpha", _alpha);
}