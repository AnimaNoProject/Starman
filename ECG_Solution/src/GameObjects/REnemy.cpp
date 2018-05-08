#include "REnemy.h"


REnemy::REnemy(Model* model, _Shader* _shader)
{
	_model = model;
	float px, py, pz;
	px = rand() % 200;
	py = rand() % 200;
	pz = rand() % 200;
	_translation = vec3(0, 0, 0);
	_position = translate(mat4(1), vec3(px, py, pz));
	_posVec = vec3(px, py, pz);
	_rotation = vec3(1, 1, 1);
	_scale = scale(mat4(1), vec3(3, 3, 3));
	_degree = 0;
	_shot = new Model("assets/objects/drone/shots.obj", _shader);
}

REnemy::REnemy(mat4 default)
{
	_defaultTransformation = default;
}

void REnemy::takeHint(vec3 position, float deltaTime)
{
	for (int i = 0; i < this->children.size(); i++)
	{
		this->children.at(i)->takeHint(position, deltaTime);
	}

	if (_model != nullptr)
	{
		//if (dot(position - _posVec, position - _posVec) <= 300)
		//{
			timepassed += deltaTime;
			if (timepassed > cooldown)
			{

				vec3 _dir(position - _posVec);

				vec3 _right(
					sin(0 - pi<float>() / 2),
					0,
					cos(0 - pi<float>() / 2)
				);

				vec3 _up(glm::cross(_right, _dir));

				_shots.push_back(new Shots(_shot, position - _posVec, _posVec, _up, _right));
				timepassed = 0;
			}

		//}
		
	}
}

REnemy::~REnemy()
{
}


void REnemy::draw()
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
	for (int i = 0; i < this->_shots.size(); i++)
	{
		_shots.at(i)->draw();
	}
}

void REnemy::addChild(REnemy* unit)
{
	this->children.push_back(unit);
}

void REnemy::setDefaultTransformation(vec3 translation, vec3 rotation, float degree)
{
	_translation = translation;
	_rotation = rotation;
	_degree = degree;
}

void REnemy::update(mat4 transformation, float time)
{
	if (_model != nullptr)
		_transformation = transformation * _position * translate(mat4(1), _translation * time) * rotate(mat4(1), _degree * time, _rotation) * _scale;

	for (int i = 0; i < this->children.size(); i++)
	{
		this->children.at(i)->update(transformation, time);
	}

	for (int i = 0; i < this->_shots.size(); i++)
	{
		this->_shots.at(i)->update(time);
	}
}
