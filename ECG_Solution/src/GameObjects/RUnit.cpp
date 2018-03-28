#include "RUnit.h"

RUnit::RUnit(Geometry* geom)
{
	mesh = geom;
}

RUnit::~RUnit()
{
}

void RUnit::draw()
{
	mesh->draw();
}

void RUnit::setPosition(glm::vec3 position)
{
	mesh->setTransformMatrix(glm::translate(glm::mat4(1), position));
}
