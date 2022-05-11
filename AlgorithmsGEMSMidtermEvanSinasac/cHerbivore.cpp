#include "cHerbivore.h"
#include <cmath>
#include <iostream>
#include "globalThings.h"

cHerbivore::cHerbivore(cSmartArray_Template<cSmartArray_Template<cSmartArray_Template<bool>>> maze)
	: cAnimal()
{
	this->m_Maze = maze;
	this->hungry = false;
	this->noFood = false;
	timer = 0.0f;
	timeTillHungry = rand() % 6 + 5;
	moveTimer = 0.0f;
	newWanderTimer = rand() % 20 + 5;
	wanderTarget = setWanderTarget();
	lastMoveDir = glm::vec3(0.0f);
	RefreshMaze();
}
cHerbivore::~cHerbivore()
{

}

void cHerbivore::Process(float dt)
{
	float moveSpeed = 5.0f;
	timer += dt;
	if (timer >= timeTillHungry && !hungry)
	{
		hungry = true;
		timer = 0.0f;
		moveTimer = 0.0f;
		//newWanderTimer = rand() % 20 + 5;
		this->noFood = true;
		for (unsigned int index = 0; index != ::plants.getSize(); index++)
		{
			if (::plants.getAtIndex(index)->GetIsMature())
			{
				this->noFood = false;
				break;
			}
		}
		RefreshMaze();
	}

	if (this->hungry && !this->noFood)
	{
		// if the herbi is hungry and there's food available
		float shortestDistance = 10000000;
		cPlantEntity* closestPlant = nullptr;
		for (unsigned int index = 0; index != ::plants.getSize(); index++)
		{
			if (glm::distance(position, ::plants.getAtIndex(index)->GetPosition()) < shortestDistance
				&& ::plants.getAtIndex(index)->GetIsMature())
			{
				shortestDistance = glm::distance(position, ::plants.getAtIndex(index)->GetPosition());
				closestPlant = ::plants.getAtIndex(index);
			}
		}
		// ok, we know where the closest plant is, let's check if we're on top of it
		if (closestPlant != nullptr)
		{
			if (glm::distance(position, closestPlant->GetPosition()) < 0.5f)
			{
				closestPlant->Eaten();
				this->hungry = false;
				this->timeTillHungry = rand() % 20 + 10; // 10 - 30
				std::cout << "Herbi goes \"CRONCH CRONCH\"" << std::endl;
			}
			else
			{	// alright, we're not close enough to the target, let's 
				// I'm too out of it for this, ughgghhghgghghhghghghg lol
				// I can't figure out how to implement a simple maze solving algorithm....
				// yeah this ain't good
				bool up = false, down = false, left = false, right = false;
				if (position.x + 1 < this->m_Maze.getSize())
				{
					right = this->m_Maze.getAtIndex(position.x + 1).getAtIndex(position.y).getAtIndex(0);
				}
				if (position.x - 1 >= 0)
				{
					left = this->m_Maze.getAtIndex(position.x - 1).getAtIndex(position.y).getAtIndex(0);
				}
				if (position.y + 1 < this->m_Maze.getAtIndex(0).getAtIndex(position.y).getSize())
				{
					up = this->m_Maze.getAtIndex(position.x).getAtIndex(position.y + 1).getAtIndex(0);
				}
				if (position.y - 1 >= this->m_Maze.getAtIndex(0).getAtIndex(position.y).getSize())
				{
					down = this->m_Maze.getAtIndex(position.x).getAtIndex(position.y - 1).getAtIndex(0);
				}
				if (right)
				{
					//lastMoveDir = glm::vec3(1.0f, 0.0f, 0.0f);
					position.x += dt * moveSpeed;
				}
				else if (left)
				{
					//lastMoveDir = glm::vec3(-1.0f, 0.0f, 0.0f);
					position.x -= dt * moveSpeed;
				}
				else if (up)
				{
					//lastMoveDir = glm::vec3(0.0f, 1.0f, 0.0f);
					position.y += dt * moveSpeed;
				}
				else if (down)
				{
					//lastMoveDir = glm::vec3(0.0f, -1.0f, 0.0f);
					position.y -= dt * moveSpeed;
				}
				//position += lastMoveDir * dt * moveSpeed;
				if (glm::distance(position, lastMoveDir) < 0.1f)
				{
					if (down)
					{
						//lastMoveDir = glm::vec3(0.0f, -1.0f, 0.0f);
						position.y -= dt * moveSpeed;
					}
					else if (up)
					{
						//lastMoveDir = glm::vec3(0.0f, 1.0f, 0.0f);
						position.y += dt * moveSpeed;
					}
					else if (left)
					{
						//lastMoveDir = glm::vec3(-1.0f, 0.0f, 0.0f);
						position.x -= dt * moveSpeed;
					}
					if (right)
					{
						//lastMoveDir = glm::vec3(1.0f, 0.0f, 0.0f);
						position.x += dt * moveSpeed;
					}
				}
			}
		}
		
	}
	else if (!this->hungry || this->noFood)
	{
		// if the herbi is not hungry or there's no food available then it will wander
		moveTimer += dt;
		if (moveTimer >= newWanderTimer)
		{
			moveTimer = 0.0f;
			newWanderTimer = rand() % 20 + 5;
			wanderTarget = setWanderTarget();
		}
		glm::vec3 dir = directionMove();
		
		if (position.x + dir.x >= this->m_Maze.getSize()
			|| position.x + dir.x <= 0.0f
			|| position.y + dir.y >= this->m_Maze.getAtIndex(0).getSize()
			|| position.y + dir.y <= 0.0f)
		{
			// idk, don't do it
		}
		else
		{
			position += dir * dt * moveSpeed;
		}
		// this is not good at all, but screw it, good enough for rock and roll, they move
		//position += dir * dt * moveSpeed;
		
		
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

	this->m_Mesh->positionXYZ = position;
	this->m_Mesh->orientationXYZ = orientation;
	this->m_Mesh->scale = scale;
}

glm::vec3 cHerbivore::setWanderTarget()
{
	float x = this->position.x + rand() % 10 + 1;
	float y = this->position.y + rand() % 10 + 1;
	if (!this->m_Maze.getAtIndex(x).getAtIndex(y).getAtIndex(0))
	{
		// if the target is sand the herbi can't go there, so look for another spot
		return setWanderTarget();
	}
	else
	{
		return glm::vec3(x, y, 0);
	}
}

glm::vec3 cHerbivore::directionMove()
{
	glm::vec3 direction = wanderTarget - position;	// direction from herbi to target
	direction = glm::normalize(direction);


	float x = direction.x, y = direction.y;
	// uuuugggggghhhhhh
	//if (direction.x < 0 && direction.x < 0.5f)
	//{
	//	x = -1.0f;
	//}
	//else if (direction.x > 0 && direction.x > 0.5f)
	//{
	//	x = 1.0f;
	//}
	//else
	//{
	//	x = 0.0f;
	//}
	//if (direction.y < 0 && direction.y < 0.5f)
	//{
	//	y = -1.0f;
	//}
	//else if (direction.y > 0 && direction.y > 0.5f)
	//{
	//	y = 1.0f;
	//}
	//else
	//{
	//	y = 0.0f;
	//}
	int currX = position.x, currY = position.y;
	// x and y are -1, 0 or 1
	// honestly, this second attempt seemed to have made them worse LMFAO
	if (abs(x) > abs(y))
	{
		if (x >= 0.0f)
		{
			if (currX + x < m_Maze.getSize())
			{
				if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
				{
					return glm::vec3(1.0f, 0.0f, 0.0f);
				}
				if (currX - x >= 0.0f)
				{
					if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
					{
						return glm::vec3(-1.0f, 0.0f, 0.0f);
					}
				}
				if (y >= 0.0f)
				{
					if (currY + y < m_Maze.getAtIndex(0).getSize())
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
						{
							return glm::vec3(0.0f, 1.0f, 0.0f);
						}
					}
					else if (currY - y >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
						{
							return glm::vec3(0.0f, -1.0f, 0.0f);
						}
					}
				}
				else if (y < 0.0f)
				{
					if (currY + y >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
						{
							return glm::vec3(0.0f, -1.0f, 0.0f);
						}
					}
					else if (currY - y < m_Maze.getAtIndex(0).getSize())
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
						{
							return glm::vec3(0.0f, 1.0f, 0.0f);
						}
					}
				}
			}
			if (currX - x >= 0.0f)
			{
				if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
				{
					return glm::vec3(-1.0f, 0.0f, 0.0f);
				}
				if (currX + x < m_Maze.getSize())
				{
					if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
					{
						return glm::vec3(1.0f, 0.0f, 0.0f);
					}
				}
				if (y >= 0.0f)
				{
					if (currY + y < m_Maze.getAtIndex(0).getSize())
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY + 1).getAtIndex(0))
						{
							return glm::vec3(0.0f, 1.0f, 0.0f);
						}
					}
					else if (currY - y > 0.0f)
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
						{
							return glm::vec3(0.0f, -1.0f, 0.0f);
						}
					}
				}
				else if (y < 0.0f)
				{
					if (currY + y >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
						{
							return glm::vec3(0.0f, -1.0f, 0.0f);
						}
					}
					else if (currY - y < m_Maze.getAtIndex(0).getSize())
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
						{
							return glm::vec3(0.0f, 1.0f, 0.0f);
						}
					}
				}
			}
		} //end of x > 0
		else if (x < 0.0f)
		{
			if (currX + x >= 0)
			{
				if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
				{
					return glm::vec3(-1.0f, 0.0f, 0.0f);
				}
				if (currX - x < m_Maze.getSize())
				{
					if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
					{
						return glm::vec3(1.0f, 0.0f, 0.0f);
					}
				}
				if (y >= 0.0f)
				{
					if (currY + y < m_Maze.getAtIndex(0).getSize())
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY + 1).getAtIndex(0))
						{
							return glm::vec3(0.0f, 1.0f, 0.0f);
						}
					}
					else if (currY - y > 0.0f)
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
						{
							return glm::vec3(0.0f, -1.0f, 0.0f);
						}
					}
				}
				else if (y < 0.0f)
				{
					if (currY + y >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
						{
							return glm::vec3(0.0f, -1.0f, 0.0f);
						}
					}
					else if (currY - y < m_Maze.getAtIndex(0).getSize())
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
						{
							return glm::vec3(0.0f, 1.0f, 0.0f);
						}
					}
				}
			}
			else if (currX - x < m_Maze.getSize())
			{
				if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
				{
					return glm::vec3(1.0f, 0.0f, 0.0f);
				}
				if (currX + x >= 0.0f)
				{
					if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
					{
						return glm::vec3(-1.0f, 0.0f, 0.0f);
					}
				}
				if (y >= 0.0f)
				{
					if (currY + y < m_Maze.getAtIndex(0).getSize())
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY + 1).getAtIndex(0))
						{
							return glm::vec3(0.0f, 1.0f, 0.0f);
						}
					}
					else if (currY - y > 0)
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
						{
							return glm::vec3(0.0f, -1.0f, 0.0f);
						}
					}
				}
				else if (y < 0.0f)
				{
					if (currY + y >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
						{
							return glm::vec3(0.0f, -1.0f, 0.0f);
						}
					}
					else if (currY - y < m_Maze.getAtIndex(0).getSize())
					{
						if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
						{
							return glm::vec3(0.0f, 1.0f, 0.0f);
						}
					}
				}
			}
		}
	}
	else
	{
		if (y >= 0)
		{
			if (currY + y < m_Maze.getAtIndex(0).getSize())
			{
				if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
				{
					return glm::vec3(0.0f, 1.0f, 0.0f);
				}
				if (currY - y >= 0)
				{
					if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
					{
						return glm::vec3(0.0f, -1.0f, 0.0f);
					}
				}
				if (x >= 0)
				{
					if (currX + x < m_Maze.getSize())
					{
						if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(1.0f, 0.0f, 0.0f);
						}
					}
					else if (currX - x >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(-1.0f, 0.0f, 0.0f);
						}
					}
				}
				else if (x < 0)
				{
					if (currX + x >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(-1.0f, 0.0f, 0.0f);
						}
					}
					else if (currX - x < m_Maze.getSize())
					{
						if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(1.0f, 0.0f, 0.0f);
						}
					}
				}
			}
			if (currY - y >= 0.0f)
			{
				if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
				{
					return glm::vec3(0.0f, -1.0f, 0.0f);
				}
				if (currY + y < m_Maze.getAtIndex(0).getSize())
				{
					if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
					{
						return glm::vec3(0.0f, 1.0f, 0.0f);
					}
				}
				if (x >= 0)
				{
					if (currX + x < m_Maze.getSize())
					{
						if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(1.0f, 0.0f, 0.0f);
						}
					}
					else if (currX - x >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(-1.0f, 0.0f, 0.0f);
						}
					}
				}
				else if (x < 0)
				{
					if (currX + x >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(-1.0f, 0.0f, 0.0f);
						}
					}
					else if (currX - x < m_Maze.getSize())
					{
						if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(1.0f, 0.0f, 0.0f);
						}
					}
				}
			}
		}	// end of y >= 0
		else if (y < 0)
		{
			if (currY + y >= 0.0f)
			{
				if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
				{
					return glm::vec3(0.0f, -1.0f, 0.0f);
				}
				if (currY - y < m_Maze.getAtIndex(0).getSize())
				{
					if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
					{
						return glm::vec3(0.0f, 1.0f, 0.0f);
					}
				}
				if (x >= 0)
				{
					if (currX + x < m_Maze.getSize())
					{
						if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(1.0f, 0.0f, 0.0f);
						}
					}
					else if (currX - x >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(-1.0f, 0.0f, 0.0f);
						}
					}
				}
				else if (x < 0)
				{
					if (currX + x >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(-1.0f, 0.0f, 0.0f);
						}
					}
					else if (currX - x < m_Maze.getSize())
					{
						if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(1.0f, 0.0f, 0.0f);
						}
					}
				}
			}
			else if (currY - y < m_Maze.getAtIndex(0).getSize())
			{
				if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
				{
					return glm::vec3(0.0f, 1.0f, 0.0f);
				}
				if (currY + y >= 0.0f)
				{
					if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
					{
						return glm::vec3(0.0f, -1.0f, 0.0f);
					}
				}
				if (x >= 0)
				{
					if (currX + x < m_Maze.getSize())
					{
						if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(1.0f, 0.0f, 0.0f);
						}
					}
					else if (currX - x >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(-1.0f, 0.0f, 0.0f);
						}
					}
				}
				else if (x < 0)
				{
					if (currX + x >= 0.0f)
					{
						if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(-1.0f, 0.0f, 0.0f);
						}
					}
					else if (currX - x < m_Maze.getSize())
					{
						if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
						{
							return glm::vec3(1.0f, 0.0f, 0.0f);
						}
					}
				}
			}
		}
	}
	
	// that should cover everything but if it doesn't work, I basically have to copy and mirror it checking y first

	// original attempt
	//if (abs(x) > abs(y))	// if x is "larger" than y then we check the x positions first
	//{
	//	if (currX + x >= m_Maze.getSize())
	//	{
	//		if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
	//		{
	//			return glm::vec3(-x, 0.0f, 0.0f);
	//			//position.x -= x * dt * moveSpeed;
	//		}
	//		else if (currY + y >= m_Maze.getAtIndex(position.x).getSize())
	//		{
	//			if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, -y, 0.0f);
	//				//position.y -= y * dt * moveSpeed;
	//			}
	//		}
	//		else if (currY - y < 0)
	//		{
	//			if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, y, 0.0f);
	//				//position.y += y * dt * moveSpeed;
	//			}
	//		}
	//		else
	//		{
	//			if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, -y, 0.0f);
	//				//position.y -= y * dt * moveSpeed;
	//			}
	//			else if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, y, 0.0f);
	//				//position.y += y * dt * moveSpeed;
	//			}
	//		}
	//	}
	//	else if (currX - x < 0)
	//	{
	//		if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
	//		{
	//			return glm::vec3(x, 0.0f, 0.0f);
	//			//position.x -= x * dt * moveSpeed;
	//		}
	//		else if (currY + y >= m_Maze.getAtIndex(position.x).getSize())
	//		{
	//			if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, -y, 0.0f);
	//				//position.y -= y * dt * moveSpeed;
	//			}
	//		}
	//		else if (currY - y < 0)
	//		{
	//			if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, y, 0.0f);
	//				//position.y += y * dt * moveSpeed;
	//			}
	//		}
	//		else
	//		{
	//			if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, -y, 0.0f);
	//				//position.y -= y * dt * moveSpeed;
	//			}
	//			else if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, y, 0.0f);
	//				//position.y += y * dt * moveSpeed;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
	//		{
	//			return glm::vec3(x, 0.0f, 0.0f);
	//			//position.y += y * dt * moveSpeed;
	//		}
	//		else if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
	//		{
	//			return glm::vec3(-x, 0.0f, 0.0f);
	//			//position.y -= y * dt * moveSpeed;
	//		}
	//		if (currY - y < 0)
	//		{
	//			if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, y, 0.0f);
	//				//position.y += y * dt * moveSpeed;
	//			}
	//		}
	//		else
	//		{
	//			if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, -y, 0.0f);
	//				//position.y -= y * dt * moveSpeed;
	//			}
	//		}
	//		if (currY + y < m_Maze.getAtIndex(0).getSize())
	//		{
	//			if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
	//			{
	//				return glm::vec3(0.0f, y, 0.0f);
	//			}
	//		}
	//	}
	//}	// checked x first
	//else
	//{
	//	if (currY + y >= m_Maze.getAtIndex(currX).getSize())
	//	{
	//		if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
	//		{
	//			return glm::vec3(0.0f, -y, 0.0f);
	//			//position.x -= x * dt * moveSpeed;
	//		}
	//		else if (currX + x >= m_Maze.getSize())
	//		{
	//			if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
	//			{
	//				return glm::vec3(-x, 0.0f, 0.0f);
	//				//position.y -= y * dt * moveSpeed;
	//			}
	//		}
	//		else if (currX - x < 0)
	//		{
	//			if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
	//			{
	//				return glm::vec3(x, 0.0f, 0.0f);
	//				//position.y += y * dt * moveSpeed;
	//			}
	//		}
	//		else
	//		{
	//			if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
	//			{
	//				return glm::vec3(-x, 0.0f, 0.0f);
	//				//position.y -= y * dt * moveSpeed;
	//			}
	//			else if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
	//			{
	//				return glm::vec3(x, 0.0f, 0.0f);
	//				//position.y += y * dt * moveSpeed;
	//			}
	//		}
	//	}
	//	else if (currY - y < 0)
	//	{
	//		if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
	//		{
	//			return glm::vec3(0.0f, y, 0.0f);
	//			//position.x -= x * dt * moveSpeed;
	//		}
	//		else if (currX + x >= m_Maze.getSize())
	//		{
	//			if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
	//			{
	//				return glm::vec3(-x, 0.0f, 0.0f);
	//				//position.y -= y * dt * moveSpeed;
	//			}
	//		}
	//		else if (currX - x < 0)
	//		{
	//			if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
	//			{
	//				return glm::vec3(x, 0.0f, 0.0f);
	//				//position.y += y * dt * moveSpeed;
	//			}
	//		}
	//		else
	//		{
	//			if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
	//			{
	//				return glm::vec3(-x, 0.0f, 0.0f);
	//				//position.y -= y * dt * moveSpeed;
	//			}
	//			else if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
	//			{
	//				return glm::vec3(x, 0.0f, 0.0f);
	//				//position.y += y * dt * moveSpeed;
	//			}
	//		}
	//	}
	//
	//}

	// if I missed something
	//if (m_Maze.getAtIndex(currX + x).getAtIndex(currY).getAtIndex(0))
	//{
	//	return glm::vec3(x, 0.0f, 0.0f);
	//	//position.y += y * dt * moveSpeed;
	//}
	//else if (m_Maze.getAtIndex(currX - x).getAtIndex(currY).getAtIndex(0))
	//{
	//	return glm::vec3(-x, 0.0f, 0.0f);
	//	//position.y -= y * dt * moveSpeed;
	//}
	//else if (m_Maze.getAtIndex(currX).getAtIndex(currY + y).getAtIndex(0))
	//{
	//	return glm::vec3(0.0f, y, 0.0f);
	//	//position.x -= x * dt * moveSpeed;
	//}
	//else if (m_Maze.getAtIndex(currX).getAtIndex(currY - y).getAtIndex(0))
	//{
	//	return glm::vec3(0.0f, -y, 0.0f);
	//	//position.x -= x * dt * moveSpeed;
	//}

	// and if I really missed everything
	return glm::vec3(0.0f);
}

void cHerbivore::RefreshMaze()
{
	if (this->m_Maze.getAtIndex(0).getAtIndex(0).getSize() <= 1)
	{
		for (unsigned int indexA = 0; indexA != this->m_Maze.getSize(); indexA++)
		{
			for (unsigned int indexB = 0; indexB != this->m_Maze.getAtIndex(indexA).getSize(); indexB++)
			{
				this->m_Maze.getAtIndex(indexA).getAtIndex(indexB).insertAtEnd(
					this->m_Maze.getAtIndex(indexA).getAtIndex(indexB).getAtIndex(0)
				);
			}
		}
	}
	else
	{
		for (unsigned int indexA = 0; indexA != this->m_Maze.getSize(); indexA++)
		{
			for (unsigned int indexB = 0; indexB != this->m_Maze.getAtIndex(indexA).getSize(); indexB++)
			{
				this->m_Maze.getAtIndex(indexA).getAtIndex(indexB).setAtIndex(1,
					this->m_Maze.getAtIndex(indexA).getAtIndex(indexB).getAtIndex(0)
				);
			}
		}
	}
}