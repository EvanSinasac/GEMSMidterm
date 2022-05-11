// Made a separate file to hold some of these cause main is getting really messyf
#include "globalThings.h"

// Question 2
void makePlants(cSmartArray_Template<cSmartArray_Template<cSmartArray_Template<bool>>> maze)
{
	for (unsigned int index = 0; index != 10; index++)
	{
		int x, y;
		x = rand() % maze.getSize();
		y = rand() % maze.getAtIndex(0).getSize();
		if (!maze.getAtIndex(x).getAtIndex(y).getAtIndex(0))
		{
			// if not a plant location try again
			index--;
			continue;
		}
		cPlantEntity* tempPlant = new cPlantEntity();
		tempPlant->SetTimeToMature(rand() % 5 + (rand() % 2 + 1));
		tempPlant->SetPosition(glm::vec3(x, y, 0.0f));
		tempPlant->SetOrientation(glm::vec3(glm::radians(270.0f), 0.0f, 0.0f));
		tempPlant->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
		//tempPlant->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
		cMesh* tempMesh = new cMesh();
		tempMesh->meshName = "SM_Generic_Tree_01.ply";
		//tempMesh->meshName = "Isosphere_Smooth_Normals.ply";
		tempMesh->positionXYZ = tempPlant->GetPosition();
		tempMesh->orientationXYZ = tempPlant->GetOrientation();
		tempMesh->scale = tempPlant->GetScale();
		tempMesh->bUseObjectDebugColour = true;
		tempMesh->bUseWholeObjectDiffuseColour = true;
		tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.2f, 1.0f, 0.2f, 1.0f);
		tempMesh->objectDebugColourRGBA = glm::vec4(0.2f, 1.0f, 0.2f, 1.0f);
		((Entity*)tempPlant)->SetMesh(tempMesh);

		::plants.insertAtEnd(tempPlant);
	}

	return;
}


void makeHerbivores(cSmartArray_Template<cSmartArray_Template<cSmartArray_Template<bool>>> maze)
{
	for (unsigned int index = 0; index < 5; index++)
	{
		int x, y;
		x = rand() % maze.getSize();
		y = rand() % maze.getAtIndex(0).getSize();
		if (!maze.getAtIndex(x).getAtIndex(y).getAtIndex(0))
		{
			// if not a grass location try again
			index--;
			continue;
		}
		cHerbivore* tempHerbi = new cHerbivore(maze);
		tempHerbi->SetPosition(glm::vec3(x, y, 0.0f));
		tempHerbi->SetOrientation(glm::vec3(glm::radians(270.0f), 0.0f, 0.0f));
		tempHerbi->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
		cMesh* tempMesh = new cMesh();
		//tempMesh->meshName = "SM_Generic_Tree_01.ply";
		tempMesh->meshName = "Isosphere_Smooth_Normals.ply";
		tempMesh->positionXYZ = tempHerbi->GetPosition();
		tempMesh->orientationXYZ = tempHerbi->GetOrientation();
		tempMesh->scale = tempHerbi->GetScale();
		tempMesh->bUseObjectDebugColour = true;
		tempMesh->bUseWholeObjectDiffuseColour = true;
		tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.2f, 0.0f, 0.8f, 1.0f);
		tempMesh->objectDebugColourRGBA = glm::vec4(0.2f, 0.0f, 0.8f, 1.0f);
		((Entity*)tempHerbi)->SetMesh(tempMesh);

		::animals.insertAtEnd(tempHerbi);
	}
}

void makeCarnivores(cSmartArray_Template<cSmartArray_Template<cSmartArray_Template<bool>>> maze)
{
	for (unsigned int index = 0; index < 3; index++)
	{
		int x, y;
		x = rand() % maze.getSize();
		y = rand() % maze.getAtIndex(0).getSize();
		if (maze.getAtIndex(x).getAtIndex(y).getAtIndex(0))
		{
			// if a grass location try again
			index--;
			continue;
		}
		cCarnivore* tempCarni = new cCarnivore(maze);
		tempCarni->SetPosition(glm::vec3(x, y, 0.0f));
		tempCarni->SetOrientation(glm::vec3(0.0f));
		tempCarni->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		cMesh* tempMesh = new cMesh();
		//tempMesh->meshName = "SM_Generic_Tree_01.ply";
		//tempMesh->meshName = "Isosphere_Smooth_Normals.ply";
		tempMesh->meshName = "Invader_Single_Cube.ply";
		tempMesh->positionXYZ = tempCarni->GetPosition();
		tempMesh->orientationXYZ = tempCarni->GetOrientation();
		tempMesh->scale = tempCarni->GetScale();
		tempMesh->bUseObjectDebugColour = true;
		tempMesh->bUseWholeObjectDiffuseColour = true;
		tempMesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 153.0f / 255.0f, 1.0f, 1.0f);
		tempMesh->objectDebugColourRGBA = glm::vec4(1.0f, 153.0f / 255.0f, 1.0f, 1.0f);
		((Entity*)tempCarni)->SetMesh(tempMesh);

		::animals.insertAtEnd(tempCarni);
	}
}
