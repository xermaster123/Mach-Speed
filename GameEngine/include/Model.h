#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{
public:
	Model(string filepath, const aiScene * scene);

	void render(const unsigned int shaderProgram);
	void loadModel(string path, const aiScene * scene);

	static unsigned int TextureFromFile(const char* filepath, const string& directory, bool gamma = false);

private:
	std::vector<Mesh> v_meshes;
	string directory;
	vector<Texture> v_textures;

	void processNode(aiNode* node, const aiScene* scene);
	void getVertices(aiMesh* mesh, const aiScene* scene, Mesh* finalMesh);
	void getIndices(aiMesh* mesh, const aiScene* scene, Mesh* finalMesh);
	vector<Texture> getTexture(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	
};
