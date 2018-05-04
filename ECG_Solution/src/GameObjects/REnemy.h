#pragma once
#include "RUnit.h"

class REnemy
{
private:
	std::vector<REnemy*> children;
	std::vector<Shots*> _shots;
	_Shader* _shader;
	Model* _shot;
	Model* _model;
	vec3 _posVec;
	float timepassed;
	float cooldown = 1.0f;
	vec3 _translation;
	vec3 _rotation;
	mat4 _scale;
	float _degree;
	mat4 _defaultTransformation;
	mat4 _position;

	mat4 _transformation;
public:
	REnemy(Model* model, _Shader* shader);
	REnemy(mat4 default);
	void takeHint(vec3 position, float deltaTime);
	~REnemy();

	virtual void draw();
	void addChild(REnemy* unit);
	virtual void setDefaultTransformation(vec3 translation, vec3 rotation, float degree);
	virtual void update(mat4 transformation, float time);
};

