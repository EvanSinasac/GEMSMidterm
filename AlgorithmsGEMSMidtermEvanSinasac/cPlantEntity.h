#pragma once

#include "Entity.h"

class cPlantEntity : Entity
{
public:
	cPlantEntity();
	virtual ~cPlantEntity();

	virtual void Process(float dt);

	void SetTimeToMature(float ttM);
	void AddFruit();

	void SetPosition(glm::vec3 pos) { this->position = pos; }
	void SetOrientation(glm::vec3 rot) { this->orientation = rot; }
	void SetScale(glm::vec3 sca) { this->scale = sca; }

	glm::vec3 GetPosition() { return this->position; }
	glm::vec3 GetOrientation() { return this->orientation; }
	glm::vec3 GetScale() { return this->scale; }

	void Eaten();
	bool GetIsMature() { return matured; }
protected:

private:
	float timeToMature;
	float timer;
	cSmartArray_Template<cPlantEntity> m_Children;	// I don't think I need this, but eh
	bool matured;
};