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
}

void RUnit::setPosition(glm::vec3 position)
{
	_geometry->setTransformMatrix(glm::translate(glm::mat4(1), position));
}
