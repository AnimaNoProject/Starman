#include "Shots.h"

Shots::Shots()
{
}

Shots::Shots(Model* model, vec3 direction, vec3 position)
{
	_model = model;

	_toofar = false;

	// set starting position
	_direction = direction;
	_position = position;
	// rotate into shooting direction
	vec3 dirA(0.0f, 1.0f, 0.0f);
	vec3 dirB(direction);
	float angle = acos(dot(dirA, dirB));
	_rotation = rotate(mat4(1), angle, cross(dirA, dirB));
	// Init Bullet collision shape
	InitPhysicProperties(_position, _direction, dirA, dirB, angle);
}

Shots::~Shots()
{
}

void Shots::InitPhysicProperties(vec3 position, vec3 direction, vec3 dirA, vec3 dirB, float angle)
{
	float numberOfVertices = 0;
	vec3 average(0, 0, 0);

	// convex hull shape
	for (unsigned int i = 0; i < _model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < _model->meshes.at(i)._vertices.size(); j++)
		{
			shapeVector.push_back(_model->meshes.at(i)._vertices.at(j).Position.x);
			shapeVector.push_back(_model->meshes.at(i)._vertices.at(j).Position.y);
			shapeVector.push_back(_model->meshes.at(i)._vertices.at(j).Position.z);
		}
	}
	_shape = new btConvexHullShape(&shapeVector[0], shapeVector.size() / 3, 3 * sizeof(btScalar));

	// Motion State
	btQuaternion rotationQuat;
	//rotationQuat.setEulerZYX(cross(dirA, dirB));

	btVector3 btDirA = btVector3(dirA.x, dirA.y, dirA.z);
	btVector3 btDirB = btVector3(dirB.x, dirB.y, dirB.z);
	rotationQuat.setRotation(btCross(btDirA, btDirB), angle);
	btVector3 positionbT = btVector3(position.x, position.y, position.z);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotationQuat, positionbT));
	//

	// Weight
	btScalar mass = 500;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(mass, bodyInertia);
	//

	// Rigid Body
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, _shape, bodyInertia);
	bodyCI.m_restitution = 0.5f;
	bodyCI.m_friction = 0.005f;
	_body = new btRigidBody(bodyCI);


	// Translation & Rotation
	_body->setLinearFactor(btVector3(1, 1, 1));
	_body->setLinearVelocity(btVector3(direction.x * 50, direction.y * 50, direction.z * 50));

	//Shots* shotPointer;
	_collisionData = new CollisionData("Shot");
	_collisionData->setParentShot(this);
	_body->setUserPointer(_collisionData);

}

long Shots::draw()
{
	_model->setTransformMatrix(translate(mat4(1), _position) * _rotation);
	return _model->Draw();
}

void Shots::update(float deltaTime)
{
	/*
	_position += _direction * (float)(deltaTime * 100.0f);
	if (distance(_startposition, _position) > 500)
	{
		_toofar = true;
	}
	*/

	btTransform transform = _body->getWorldTransform();
	btQuaternion rota = transform.getRotation();
	_position = vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());


	if (distance(_startposition, _position) > 200)
	{
		_toofar = true;
	}
}