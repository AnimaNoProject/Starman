#pragma once
#include "RUnit.h"

class CollisionData;

class REnemy
{
private:
	CollisionData * _collisionData;


	btConvexHullShape* _shape;
	_Shader* _shader;

	Model* _shot;
	Model* _model;

	std::vector<REnemy*> children;
	std::vector<Shots*> _shots;
	vector<btScalar> shapeVector;

	float timepassed;
	float cooldown = 1.0f;

	vec3 _translation, _rotation, _middle, _posVec, mostDistant;
	mat4 _scale, _position, _transformation;

	float _degree, radius, _scaleFactor;

	void InitPhysicProperties(vec3 position, vec3 translation, vec3 rotation, float degree, mat4 scale, float weight);
	void InitRandom();
public:
	btRigidBody * _body;
	REnemy(Model* model, _Shader* shader);
	REnemy(mat4 default);
	void takeHint(vec3 position, float deltaTime);
	~REnemy();

	float health = 100;

	virtual long draw();
	void addChild(REnemy* unit);
	virtual void update(mat4 transformation, float time);
};

