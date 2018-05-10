#pragma once
#include "../Rendering/Model.h"
#include "Shots.h"
#include "btBulletDynamicsCommon.h"

class RUnit
{
private:
	std::vector<RUnit*> children;
protected:
	vec3 _translation;
	vec3 _rotation;
	mat4 _scale;
	float _degree;
	mat4 _defaultTransformation;
	mat4 _position;
	Model* _model;
	mat4 _transformation;
public:
	btRigidBody * _body;
	btCollisionShape* _shape;
	RUnit(Model* model, vec3 translation, vec3 rotation, float degree, vec3 position);
	RUnit(Model* model);
	RUnit(mat4 defaultTransformation);
	RUnit();
	~RUnit();
	virtual long draw();
	void addChild(RUnit* unit);
	virtual void setDefaultTransformation(vec3 translation, vec3 rotation, float degree);
	virtual void update(mat4 transformation, float time);
};

