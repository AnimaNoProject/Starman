#include "RUnit.h"

RUnit::RUnit(Model * model, vec3 translation, vec3 rotation, float degree, vec3 position)
{
	_model = model;
	_translation = translation;
	_rotation = rotation;
	_degree = degree;
	_position = translate(mat4(1), position);
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
	if(_model != nullptr)
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
	if(_model != nullptr)
		_model->setTransformMatrix(transformation * _position * translate(mat4(1), _translation * time) * rotate(mat4(1), _degree * time, _rotation));
	for (int i = 0; i < this->children.size(); i++)
	{
		this->children.at(i)->update(transformation, time);
	}
}
