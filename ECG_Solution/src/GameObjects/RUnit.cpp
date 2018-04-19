#include "RUnit.h"

RUnit::RUnit(Model* model)
{
	_model = model;
}

RUnit::RUnit()
{
}

RUnit::~RUnit()
{
}

void RUnit::draw()
{
	_model->Draw();
	for (RUnit unit : children)
	{
		unit.draw();
	}
}

void RUnit::setTransformation(glm::mat4 transformation)
{
	this->transformation = transformation;
	_model->setTransformMatrix(transformation);
}

void RUnit::addChild(RUnit unit)
{
	children.push_back(unit);
}

void RUnit::setTime(float deltaTime)
{
	for (RUnit unit : children)
	{
		unit.setTime(deltaTime);
	}
}

void RUnit::setMovement(vec3 translation, vec3 rotation)
{
	_model->setTransformMatrix(rotate(mat4(1), deltaTime * 20, rotation) * translate(glm::mat4(1), rotation * deltaTime));
}
