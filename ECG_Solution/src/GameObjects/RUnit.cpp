#include "RUnit.h"

RUnit::RUnit(Model * model, vec3 translation, vec3 rotation, float degree, vec3 position)
{
	_model = model;
	_translation = translation;
	_rotation = rotation;
	_degree = degree;
	_position = translate(mat4(1), position);
}

RUnit::RUnit(Model * model)
{
	_model = model;
	float px, py, pz, r, tx, ty, tz, rx, ry, rz, s;
	tx = rand() & 1;
	ty = rand() & 1;
	tz = rand() & 1;
	px = rand() % 200;
	py = rand() % 200;
	pz = rand() % 200;
	rx = rand() & 1;
	ry = rand() & 1;
	rz = rand() & 1;
	s = rand() % 5;
	r = rand() % 25 / 100;
	_translation = vec3(tx, ty, tz);
	_position = translate(mat4(1), vec3(px, py, pz));
	_rotation = vec3(rx, ry, rz);
	_scale = scale(mat4(1), vec3(s, s, s));
	_degree = r;
}

RUnit::RUnit(mat4 defaultTransformation)
{
	_defaultTransformation = defaultTransformation;
}

RUnit::RUnit()
{
}

RUnit::~RUnit()
{
}

void RUnit::draw()
{
	if (_model != nullptr)
	{
		_model->setTransformMatrix(_transformation);
		_model->Draw();
	}
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
		_transformation = transformation * _position * translate(mat4(1), _translation * time) * rotate(mat4(1), _degree * time, _rotation) * _scale;
	for (int i = 0; i < this->children.size(); i++)
	{
		this->children.at(i)->update(transformation, time);
	}
}
