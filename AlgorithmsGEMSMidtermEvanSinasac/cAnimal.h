#pragma once

#include "Entity.h"

class cAnimal : public Entity
{
public:
	cAnimal() : Entity() {};
	virtual ~cAnimal() {};

	virtual void Process(float dt) {};
};