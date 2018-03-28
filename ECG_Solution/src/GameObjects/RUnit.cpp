#include "RUnit.h"

RUnit::RUnit(Geometry geom)
{
	mesh = geom;
}

RUnit::~RUnit()
{
}

void RUnit::draw()
{
	mesh.draw();
}

void RUnit::setPosition(glm::vec3 position)
{
	mesh.transform(glm::translate(glm::mat4(1), position));
}
