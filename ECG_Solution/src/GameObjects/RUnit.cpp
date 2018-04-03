#include "RUnit.h"

RUnit::RUnit(Geometry* geometry)
{
	_geometry = geometry;
}

RUnit::RUnit()
{
}

RUnit::~RUnit()
{
}

void RUnit::draw()
{
	_geometry->draw();
	for (RUnit unit : children)
	{
		unit.draw();
	}
}

void RUnit::setTransformation(glm::mat4 transformation)
{
	this->transformation = transformation;
	_geometry->setTransformMatrix(transformation);
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

void RUnit::setMovement(glm::vec3 translation, glm::vec3 rotation)
{
	_geometry->setTransformMatrix(glm::rotate(glm::mat4(1), deltaTime * 20, rotation) * glm::translate(glm::mat4(1), rotation * deltaTime));
}
