#pragma once

#include "RUnit.h"
#include <string>
#include <utility>

class RUnit;
class Shots;
class RPlayer;

class CollisionData
{
private:
	string _type;
public:
	RUnit * _parentRUnit;
	Shots* _parentShot;
	RPlayer* _parentPlayer;
	CollisionData(string t);
	~CollisionData();

	string getType();
	
	void setParentRUnit(RUnit* unit);
	void setParentShot(Shots* shot);
	void setParentPlayer(RPlayer* player);
};
