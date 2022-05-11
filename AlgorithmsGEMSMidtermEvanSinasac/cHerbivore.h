#pragma once

#include "cAnimal.h"

class cHerbivore : public cAnimal
{
public:
	cHerbivore(cSmartArray_Template<cSmartArray_Template<cSmartArray_Template<bool>>> maze);
	virtual ~cHerbivore();

	virtual void Process(float dt);

	

	void SetPosition(glm::vec3 pos) { this->position = pos; }
	void SetOrientation(glm::vec3 rot) { this->orientation = rot; }
	void SetScale(glm::vec3 sca) { this->scale = sca; }

	glm::vec3 GetPosition() { return this->position; }
	glm::vec3 GetOrientation() { return this->orientation; }
	glm::vec3 GetScale() { return this->scale; }
protected:

private:
	cSmartArray_Template<cHerbivore> m_Children;	// for future
	bool hungry;
	bool noFood;
	cSmartArray_Template<cSmartArray_Template<cSmartArray_Template<bool>>> m_Maze;	// for navigating the maze
	float timeTillHungry;
	float timer;
	float moveTimer;
	float newWanderTimer;
	glm::vec3 wanderTarget;

	glm::vec3 setWanderTarget();
	glm::vec3 directionMove();
	void RefreshMaze();
	glm::vec3 lastMoveDir;
};