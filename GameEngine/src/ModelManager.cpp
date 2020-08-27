#include "ModelManager.h"
#include <iostream>
#include <assimp/postprocess.h>

ModelManager::~ModelManager()
{
	for (auto it = loadedModels.begin(); it != loadedModels.end(); ++it)
	{
		delete (it->second);
	}
	loadedModels.clear();
}

bool ModelManager::loadModel(const std::string & filePath)
{
	// Check to see if this model is already loaded
	if (getRef(filePath)) return true;

	// Check to see if the file path is correct & load if it is
	const aiScene * scene = importer.ReadFile(assetPath + filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return false;
	}

	// Load and store model in memory in our array
	loadedModels[filePath] = new Model(assetPath + filePath, scene);
	return true;
}

void ModelManager::deleteModel(const std::string & filePath)
{
	auto it = loadedModels.find(filePath);

	if (it != std::end(loadedModels))
	{
		loadedModels.erase(it);
	}
}

Model* ModelManager::getRef(const std::string & filePath)
{
	Model* model;

	// Try to find the model with specified name
	auto it = loadedModels.find(filePath);

	if (it != std::end(loadedModels))
	{
		// If the model was successfully retreived return it
		return it->second;
	}

	// If there is no model return nullptr
	return nullptr;
}
