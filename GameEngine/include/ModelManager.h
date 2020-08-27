#pragma once
#include "Model.h"
#include <map>

class ModelManager
{
public:
	~ModelManager();

	/// Load the model into memory.
	bool loadModel(const std::string & filePath);
	/// Delete the model from memory.
	void deleteModel(const std::string & filePath);

	/// Get a reference to the specified model name.
	/// Returns null if there is not such file currently loaded.
	Model * getRef(const std::string& filePath);

private:
	std::string assetPath = "assets/models/";
	map<std::string, Model *> loadedModels;
	Assimp::Importer importer;
};
