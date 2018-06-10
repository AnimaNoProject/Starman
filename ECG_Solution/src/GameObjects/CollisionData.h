#pragma once

#include "RUnit.h"
#include <string>
#include <utility>

class RUnit;
class Shots;
class RPlayer;

class CollisionData
{
public:
	enum TYPE { SHOT, PLAYER, PICKUP, ASTEROID, ENEMY };
	RUnit* _parentRUnit;
	Shots* _parentShot;
	RPlayer* _parentPlayer;
	TYPE _type;
	CollisionData(TYPE type);
	~CollisionData();
};
