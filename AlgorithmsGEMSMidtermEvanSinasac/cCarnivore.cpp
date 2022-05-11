#include "cCarnivore.h"
#include <iostream>
#include "globalThings.h"

cCarnivore::cCarnivore(cSmartArray_Template<cSmartArray_Template<cSmartArray_Template<bool>>> maze)
	: cAnimal()
{
	this->m_Maze = maze;
	hungry = false;
	noFood = false;
	sleeping = false;
	sleepTimer = 0.0f;
	maxSleepTime = rand() % 6 + 1;
	timeTillHungry = rand() % 11 + 10;
	timer = 0.0f;
	direction = directionMove();
	untilMove = 2.0f;
	moveTimer = 0.0f;
}
cCarnivore::~cCarnivore()
{

}

void cCarnivore::Process(float dt)
{
	float moveSpeed = 10.0f;
	if (!this->sleeping)
	{
		timer += dt;
		if (timer >= timeTillHungry)
		{
			hungry = true;
			timer = 0.0f;
			if (::animals.getSize() <= 3)
			{
				this->noFood = true;
			}
		}

		if (this->hungry && !this->noFood)
		{
			// if the carni is hungry and there's food available
			float shortestDistance = 10000000;
			cHerbivore* closestPrey = nullptr;
			for (unsigned int index = 0; index != ::animals.getSize() - 3; index++)
			{
				if (glm::distance(position, ((cHerbivore*)::animals.getAtIndex(index))->GetPosition()) < shortestDistance)
				{
					shortestDistance = glm::distance(position, ((cHerbivore*)::animals.getAtIndex(index))->GetPosition());
					closestPrey = ((cHerbivore*)::animals.getAtIndex(index));
				}
			}
			if (closestPrey != nullptr)
			{
				if (glm::distance(position, closestPrey->GetPosition()) < 0.5f)
				{
					this->hungry = false;
					this->timeTillHungry = rand() % 20 + 30; // 10 - 30
					std::cout << "Carni goes \"NOM\"" << std::endl;
				}
				else
				{
					direction = glm::vec3(0.0f);
					if (abs(position.x - closestPrey->GetPosition().x) > abs(position.y - closestPrey->GetPosition().y))
					{
						if (position.x > closestPrey->GetPosition().x)
						{
							// we're farther right (left), so we'll move in -x
							direction = glm::vec3(-1.0f, 0.0f, 0.0f);
						}
						else
						{
							direction = glm::vec3(1.0f, 0.0f, 0.0f);
						}
					}
					else
					{
						if (position.y > closestPrey->GetPosition().y)
						{
							direction = glm::vec3(0.0f, -1.0f, 0.0f);
						}
						else
						{
							direction = glm::vec3(0.0f, 1.0f, 0.0f);
						}
					}
				}
				// this is good at all, but screw it, good enough for rock and roll, they move
				if (position.x + direction.x >= this->m_Maze.getSize()
					|| position.x + direction.x <= 0.0f
					|| position.y + direction.y >= this->m_Maze.getAtIndex(0).getSize()
					|| position.y + direction.y <= 0.0f)
				{
					// idk, don't do it
				}
				else
				{
					position += direction * dt * moveSpeed;
				}

				if (position.x >= this->m_Maze.getSize())
				{
					position.x = this->m_Maze.getSize() - 1;
				}
				if (position.x < 0.0f)
				{
					position.x = 0.0f;
				}
				if (position.y >= this->m_Maze.getAtIndex(0).getSize())
				{
					position.y = this->m_Maze.getAtIndex(0).getSize() - 1;
				}
				if (position.y < 0.0f)
				{
					position.y = 0.0f;
				}
			}

		}
		else if (!this->hungry || this->noFood)
		{
			// if the carni is not hungry or there's no food available then it will wander

			moveTimer += dt;
			if (moveTimer >= untilMove)
			{
				moveTimer = 0.0f;
				direction = directionMove();
			}
			
			//glm::vec3 dir = directionMove();

			// this is good at all, but screw it, good enough for rock and roll, they move
			if (position.x + direction.x >= this->m_Maze.getSize()
				|| position.x + direction.x <= 0.0f
				|| position.y + direction.y >= this->m_Maze.getAtIndex(0).getSize()
				|| position.y + direction.y <= 0.0f)
			{
				// idk, don't do it
			}
			else
			{
				position += direction * dt * moveSpeed;
			}
			
			if (position.x >= this->m_Maze.getSize())
			{
				position.x = this->m_Maze.getSize() - 1;
			}
			if (position.x < 0.0f)
			{
				position.x = 0.0f;
			}
			if (position.y >= this->m_Maze.getAtIndex(0).getSize())
			{
				position.y = this->m_Maze.getAtIndex(0).getSize() - 1;
			}
			if (position.y < 0.0f)
			{
				position.y = 0.0f;
			}

			if (this->m_Maze.getAtIndex(position.x).getAtIndex(position.y).getAtIndex(0))
			{
				int slep = rand() % 100 + 1;
				if (slep <= 25)
				{
					sleeping = true;
					sleepTimer = 0.0f;
					maxSleepTime = rand() % 6 + 1;
					this->m_Mesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					this->m_Mesh->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}
		}
	}
	else
	{
		sleepTimer += dt;
		if (sleepTimer >= maxSleepTime)
		{
			//std::cout << "SLEEPING" << std::endl;
			sleeping = false;
			sleepTimer = 0.0f;
			this->m_Mesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 153.0f / 255.0f, 1.0f, 1.0f);
			this->m_Mesh->objectDebugColourRGBA = glm::vec4(1.0f, 153.0f / 255.0f, 1.0f, 1.0f);
		}
	}
	
	this->m_Mesh->positionXYZ = position;
	this->m_Mesh->orientationXYZ = orientation;
	this->m_Mesh->scale = scale;
}


glm::vec3 cCarnivore::directionMove()
{
	glm::vec3 moveTo = glm::vec3(0.0f);
	int currX = position.x, currY = position.y;
	int randomDir = rand() % 4;
	switch (randomDir)
	{
	case 0:	// move right
		if (currX + 1 < this->m_Maze.getSize())
		{
			moveTo = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		break;
	case 1:	//move left
		if (currX - 1 >= 1)
		{
			moveTo = glm::vec3(-1.0f, 0.0f, 0.0f);
		}
		break;
	case 2:	// move up
		if (currY + 1 < this->m_Maze.getAtIndex(0).getSize())
		{
			moveTo = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		break;
	case 3:	// move down
		if (currY - 1 >= 1)
		{
			moveTo = glm::vec3(0.0f, -1.0f, 0.0f);
		}
		break;
	default:
		break;
	}

	// and if I really missed everything
	return moveTo;
}