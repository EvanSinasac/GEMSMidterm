#pragma once

#include "cAnimal.h"

class cCarnivore : public cAnimal
{
public:
	cCarnivore(cSmartArray_Template<cSmartArray_Template<cSmartArray_Template<bool>>> maze);
	virtual ~cCarnivore();

	virtual void Process(float dt);


	void SetPosition(glm::vec3 pos) { this->position = pos; }
	void SetOrientation(glm::vec3 rot) { this->orientation = rot; }
	void SetScale(glm::vec3 sca) { this->scale = sca; }

	glm::vec3 GetPosition() { return this->position; }
	glm::vec3 GetOrientation() { return this->orientation; }
	glm::vec3 GetScale() { return this->scale; }

protected:

private:
	cSmartArray_Template<cCarnivore> m_Children;	// for future
	bool hungry;
	bool noFood;
	bool sleeping;
	float sleepTimer;
	float maxSleepTime;
	cSmartArray_Template<cSmartArray_Template<cSmartArray_Template<bool>>> m_Maze;	// for navigating the maze
	float timeTillHungry;
	float timer;
	float moveTimer;
	float untilMove;

	glm::vec3 direction;

	glm::vec3 directionMove();
};