#pragma once

#include "RUnit.h"
#include <string>
#include <utility>

class RUnit;

class CollisionData
{
private:
	string _type;
	RUnit* _parentRUnit;
	
public:
	CollisionData(string t);
	~CollisionData();

	string getType();
	
	void setParentRUnit(RUnit* _p);
};
