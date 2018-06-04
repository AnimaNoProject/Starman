#pragma once
#include "RUnit.h"
#include <string>


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
