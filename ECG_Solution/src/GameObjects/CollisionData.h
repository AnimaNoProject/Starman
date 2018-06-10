#pragma once

#include "RUnit.h"
#include <string>
#include <utility>

class RUnit;
class Shots;
class RPlayer;
class REnemy;

enum TYPE { SHOT = 0, PLAYER, PICKUP, ASTEROID, ENEMY };

class CollisionData
{
public:
	RUnit* _parentRUnit;
	Shots* _parentShot;
	RPlayer* _parentPlayer;
	REnemy* _parentEnemy;
	TYPE _type;
	CollisionData(TYPE type);
	~CollisionData();
};
