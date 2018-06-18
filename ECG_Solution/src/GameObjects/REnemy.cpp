#include "REnemy.h"


REnemy::REnemy(Model* model, _Shader* shader) : health(100)
{
	_model = model;
	_shader = shader;
	_shot = new Model("assets/objects/drone/shots.obj", _shader, false);
	timepassed = 0;
	InitRandom();
}

void REnemy::InitRandom()
{
	// Generate Random Properties
	//vec3 position(Random::randomNumber(-501, 501), Random::randomNumber(-501, 501), Random::randomNumber(-501, 501));
	//vec3 translation(Random::randomNumber(-10, 10), Random::randomNumber(-10, 10), Random::randomNumber(-10, 10));
	//vec3 rotation(Random::randomNumber(0, 1), Random::randomNumber(0, 1), Random::randomNumber(0, 1));
	vec3 position(0, 0, 0);
	vec3 translation(0, 0, 0);
	vec3 rotation(0, 0, 0);
	float scaleFactor = Random::randomNumber(8, 8);
	float degree = Random::randomNumber(1, 180);
	float weight = scaleFactor * 100;
	_scaleFactor = scaleFactor;
	//
	_scale = scale(mat4(1), vec3(scaleFactor, scaleFactor, scaleFactor));
	//

	// Init Physics
	InitPhysicProperties(position, translation, rotation, degree, _scale, weight);
}

void REnemy::InitPhysicProperties(vec3 position, vec3 translation, vec3 rotation, float degree, mat4 scale, float weight)
{
	float numberOfVertices = 0;
	vec3 average(0, 0, 0);

	for (unsigned int i = 0; i < _model->meshes.size(); i++)
	{
		numberOfVertices += _model->meshes.at(i)._vertices.size();
	}

	// convex hull shape
	for (unsigned int i = 0; i < _model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < _model->meshes.at(i)._vertices.size(); j++)
		{
			vec3 newPos = scale * vec4(_model->meshes.at(i)._vertices.at(j).Position, 1.0f);
			shapeVector.push_back(newPos.x);
			shapeVector.push_back(newPos.y);
			shapeVector.push_back(newPos.z);

			average += _model->meshes.at(i)._vertices.at(j).Position * (1 / numberOfVertices);
		}
	}
	_middle = average;
	_shape = new btConvexHullShape(&shapeVector[0], shapeVector.size() / 3, 3 * sizeof(btScalar));
	//

	// radius of bounding sphere
	float farthest = 0.0f;
	for (unsigned int i = 0; i < _model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < _model->meshes.at(i)._vertices.size(); j++)
		{
			vec3 newPos = _model->meshes.at(i)._vertices.at(j).Position;
			float dist = distance(newPos, _middle);
			if (dist > farthest)
			{
				farthest = dist;
				mostDistant = _model->meshes.at(i)._vertices.at(j).Position;
			}
		}
	}

	radius = farthest;

	//

	// Motion State
	btQuaternion rotationQuat;
	rotationQuat.setEulerZYX(rotation.x, rotation.y, rotation.z);
	btVector3 positionbT = btVector3(position.x, position.y, position.z);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotationQuat, positionbT));
	//

	// Weight
	btScalar mass = weight;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(mass, bodyInertia);
	//

	// Rigid Body
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, _shape, bodyInertia);
	bodyCI.m_restitution = 0.5f;
	bodyCI.m_friction = 0.005f;
	_body = new btRigidBody(bodyCI);
	//
	_collisionData = new CollisionData(ENEMY);
	_collisionData->_parentEnemy = this;
	_body->setUserPointer(_collisionData);

	// Translation & Rotation
	_body->setLinearFactor(btVector3(1, 1, 1));
	_body->setLinearVelocity(btVector3(translation.x, translation.y, translation.z));
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
		timepassed += deltaTime;
		vec4 temp_middle = vec4(_middle, 1);
		temp_middle = _transformation * temp_middle;
		vec3 tempPos = temp_middle;
		if(distance(tempPos, position) < 1000)
		{
			if (timepassed > 15)
			{

				vec3 _dir(position - tempPos);

				vec3 _right(
					sin(0 - pi<float>() / 2),
					0,
					cos(0 - pi<float>() / 2)
				);

				vec3 _up(glm::cross(_right, _dir));
				Shots* shot = new Shots(_shot, _dir, tempPos + (15.0f*_dir));
				_shots.push_back(shot);
				_world->addRigidBody(shot->_body);
				timepassed = 0;
			}
		}
	}
}

REnemy::~REnemy()
{
}


void REnemy::deleteMe()
{
	for (int i = _shots.size() - 1; i >= 0; i--)
	{
		_world->removeRigidBody(_shots.at(i)->_body);
		_shots.erase(_shots.begin() + i);
	}
}

long REnemy::draw(Frustum* frustum)
{
	unsigned int triangles = 0;
	if (_model != nullptr)
	{
		_model->setTransformMatrix(_transformation);

		vec4 middle = _transformation * vec4(_middle, 1);
		vec3 temp_dist = _transformation * vec4(mostDistant, 1);
		vec3 temp_middle = vec3(middle.x, middle.y, middle.z);

		float temp_radius = distance(temp_middle, temp_dist);

		if (frustum->Inside(temp_middle, temp_radius))
			triangles += _model->Draw();
	}
	for (int i = 0; i < this->children.size(); i++)
	{
		triangles += this->children.at(i)->draw(frustum);
	}
	
	for (int i = 0; i < this->_shots.size(); i++)
	{
		triangles += _shots.at(i)->draw();
	}

	return triangles;
}

void REnemy::addChild(REnemy* unit)
{
	//unit->_world = _world;
	this->children.push_back(unit);
}

void REnemy::update(mat4 transformation, float time)
{
	if (_model != nullptr)
	{
		btTransform transform = _body->getWorldTransform();
		btQuaternion rota = transform.getRotation();
		_translation = vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
		_posVec = _translation;
		_rotation = vec3(rota.getX(), rota.getY(), rota.getZ());
		_degree = rota.getAngle();
		_transformation = translate(mat4(1), vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z())) * rotate(mat4(1), _degree, _rotation) * _scale;
	}

	for (int i = this->children.size()-1; i >= 0; i--)
	{
		this->children.at(i)->update(transformation, time);

		if (children.at(i)->health < 0)
		{
			children.at(i)->deleteMe();
			_world->removeRigidBody(children.at(i)->_body);
			children.erase(children.begin() + i);
		}
	}

	for (int i = _shots.size() - 1; i >= 0; i--)
	{
		if (_shots.at(i)->_toofar)
		{
			_world->removeRigidBody(_shots.at(i)->_body);
			_shots.erase(_shots.begin() + i);
		}
		else
		{
			this->_shots.at(i)->update(time);
		}
	}
}
