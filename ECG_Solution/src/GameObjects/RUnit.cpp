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

void RUnit::setPosition(glm::vec3 position)
{
	_model->setTransformMatrix(glm::translate(glm::mat4(1), position));
}

void RUnit::addChild(RUnit unit)
{
	children.push_back(unit);
}
