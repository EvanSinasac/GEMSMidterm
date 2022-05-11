#pragma once
#include "cMesh.h"
#include "GLMCommon.h"
#include "cSmartArray_Template.h"		// no std vector for me from this point on

// base entity class

// what will every entity need?
class Entity
{
public:
	Entity() {}
	virtual ~Entity() {}

	virtual void Process(float deltaTime) {}

	cMesh* GetMesh() { return this->m_Mesh; }
	void SetMesh(cMesh* mesh) { this->m_Mesh = mesh; }
	
	cMesh* m_Mesh;
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 scale;
protected:

private:
	
};