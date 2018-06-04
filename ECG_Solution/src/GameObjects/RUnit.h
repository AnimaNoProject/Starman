#pragma once
#include "../Rendering/Model.h"
#include "Shots.h"
#include "btBulletDynamicsCommon.h"
#include "../Rendering/Frustum.h"
#include "../Utility/Random.h"
#include "CollisionData.h"

class RUnit
{
private:
	CollisionData* _collisionData;
protected:
	Model* _model;

	vec3 _translation, _rotation, _middle;
	mat4 _scale, _position, _transformation;
	float _degree, radius;

	void InitPhysicProperties(vec3 position, vec3 translation, vec3 rotation, float degree, mat4 scale, float weight);
public:
	std::vector<RUnit*> children;
	btRigidBody* _body;
	btConvexHullShape* _shape;
	vector<btScalar> shapeVector;

	RUnit(Model* model, vec3 position, vec3 translation, vec3 rotation, float degree, vec3 scale, float weight);
	RUnit(Model* model);

	void InitRandom();

	RUnit();
	~RUnit();

	void addChild(RUnit* unit);
	virtual long draw(Frustum* frustum);
	virtual void update(mat4 transformation, float time);
};

