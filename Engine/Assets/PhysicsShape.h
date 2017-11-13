#pragma once
#include "Resource.h"
#include "Physics\Physics.h"
class PhysicsShape : public Resource<PhysicsShape>
{
public:
	struct Data : public Shape
	{
	};

public:
	PhysicsShape(StringRef filename)
		: Base(filename)
	{}

private:
	friend bool Load(StringRef filename, Data& data)
	{
		// TODO: Implement load logic

		return false;
	}
};