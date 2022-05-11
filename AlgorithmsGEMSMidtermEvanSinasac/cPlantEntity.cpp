#include "cPlantEntity.h"

cPlantEntity::cPlantEntity() : Entity()
{
	matured = false;
	timer = 0.0f;
}
cPlantEntity::~cPlantEntity()
{

}

void cPlantEntity::Process(float dt)
{
	if (!matured)
	{
		timer += dt;
		scale.y += dt / 100.0f;// *10.0f;
		if (timer >= timeToMature)
		{
			AddFruit();
			matured = true;
		}
	}
	this->m_Mesh->positionXYZ = position;
	this->m_Mesh->orientationXYZ = orientation;
	this->m_Mesh->scale = scale;
}

void cPlantEntity::SetTimeToMature(float ttM)
{
	timeToMature = ttM;
	return;
}
void cPlantEntity::AddFruit()
{
	// Make a new mesh near the top of the tree
	// or just change the colour, colour will be better for resources
	this->m_Mesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	this->m_Mesh->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void cPlantEntity::Eaten()
{
	this->matured = false;
	this->m_Mesh->scale.y = 0.01f;
	this->m_Mesh->wholeObjectDiffuseRGBA = glm::vec4(0.2f, 0.0f, 0.2f, 1.0f);
	this->m_Mesh->objectDebugColourRGBA = glm::vec4(0.2f, 0.0f, 0.2f, 1.0f);
	this->SetTimeToMature(rand() % 5 + (rand() % 2 + 1));

}