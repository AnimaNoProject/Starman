#include "REnemy.h"


REnemy::REnemy(Model* model, _Shader* _shader)
{
	_model = model;
	float px, py, pz;
	px = rand() % 200;
	py = rand() % 200;
	pz = rand() % 200;
	_posVec = vec3(px, py, pz);
	_position = translate(mat4(1), _posVec);
	_degree = 0;
	_shot = new Model("assets/objects/drone/shots.obj", _shader);
}

REnemy::REnemy(mat4 default)
{
}

void REnemy::takeHint(vec3 position, float deltaTime)
{
	for (int i = 0; i < this->children.size(); i++)
	{
		this->children.at(i)->takeHint(position, deltaTime);
	}

	if (_model != nullptr)
	{
		if(distance(position, _posVec) < 7000)
		{
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

				_shots.push_back(new Shots(_shot, normalize(_dir), _posVec));
				timepassed = 0;
			}
		}
		
	}
}

REnemy::~REnemy()
{
}


long REnemy::draw()
{
	unsigned int triangles = 0;
	if (_model != nullptr)
	{
		_model->setTransformMatrix(_transformation);
		triangles += _model->Draw();
	}
	for (int i = 0; i < this->children.size(); i++)
	{
		triangles += this->children.at(i)->draw();
	}
	for (int i = 0; i < this->_shots.size(); i++)
	{
		triangles += _shots.at(i)->draw();
	}
	return triangles;
}

void REnemy::addChild(REnemy* unit)
{
	this->children.push_back(unit);
}

void REnemy::update(mat4 transformation, float time)
{
	if (_model != nullptr)
		_transformation = transformation * _position;

	for (int i = 0; i < this->children.size(); i++)
	{
		this->children.at(i)->update(transformation, time);
	}

	for (int i = 0; i < this->_shots.size(); i++)
	{
		this->_shots.at(i)->update(time);
	}

	for (int i = _shots.size() - 1; i >= 0; i--)
	{
		if (_shots.at(i)->_toofar)
			_shots.erase(_shots.begin() + i);
	}
}
