#include "RUnit.h"

RUnit::RUnit(Model * model, vec3 translation, vec3 rotation, float degree)
{
	_model = model;
	_translation = translation;
	_rotation = rotation;
	_degree = degree;
}

RUnit::RUnit(mat4 defaultTransformation)
{
	_defaultTransformation = defaultTransformation;
}

RUnit::~RUnit()
{
}

void RUnit::draw()
{
	_model->Draw();
	for (int i = 0; i < this->children.size(); i++)
	{
		this->children.at(i)->draw();
	}
}

void RUnit::addChild(RUnit* unit)
{
	this->children.push_back(unit);
}

void RUnit::setDefaultTransformation(vec3 translation, vec3 rotation, float degree)
{
	_translation = translation;
	_rotation = rotation;
	_degree = degree;
}

void RUnit::update(mat4 transformation, float time)
{
	_model->setTransformMatrix(transformation * translate(mat4(1), _translation * time) * rotate(mat4(1), _degree * time, _rotation));
	for (int i = 0; i < this->children.size(); i++)
	{
		this->children.at(i)->update(transformation, time);
	}
}
