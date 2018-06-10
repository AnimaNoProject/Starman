#pragma once

#include "../Rendering/Model.h";
#include "btBulletDynamicsCommon.h"
#include "CollisionData.h"

class CollisionData;

class Shots
{
private:
	Model* _model;
	vec3 _position;
	vec3 _direction;
	vec3 _startposition;
	mat4 _rotation;
	const unsigned int damage = 2;
	CollisionData* _collisionData;

	void InitPhysicProperties(vec3 position, vec3 translation, vec3 dirA, vec3 dirB, float angle);

public:
	btRigidBody* _body;
	btConvexHullShape* _shape;
	vector<btScalar> shapeVector;

	bool _toofar;
	bool _collisionFlag;
	Shots();
	Shots(Model* model, vec3 direction, vec3 position);
	~Shots();
	long draw();
	void update(float deltaTime);
};

