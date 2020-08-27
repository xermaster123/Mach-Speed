#include "Model.h"
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
#include <iostream>
#include <future>

Model::Model(string filepath, const aiScene * scene)
{
	loadModel(filepath, scene);
}

void Model::loadModel(string filepath, const aiScene * scene)
{
	//Assimp::Importer* import = new Assimp::Importer;
	//
	//std::async(&Model::readModelFile, this, filepath, import);
	//directory = filepath.substr(0, filepath.find_last_of('/'));
	//v_importers.push_back(import);

	directory = filepath.substr(0, filepath.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// get the meshes of the node and add them to our vector
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{ 
		int sceneMeshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[sceneMeshIndex];
		Mesh finalMesh = Mesh();

		std::async(&Model::getVertices, this, mesh, scene, &finalMesh);
		std::async(&Model::getIndices, this, mesh, scene, &finalMesh);
		
		vector<Texture> meshTextures = getTexture(mesh, scene);
		//auto finalVertices = meshVertices.get();
		//auto finalIndices = meshIndices.get();

		finalMesh.textures = meshTextures;
		//finalMesh.vertices = finalVertices;
		//finalMesh.indices = finalIndices;
		//finalMesh.setupMesh();
		
		v_meshes.push_back(finalMesh);

	}
	// recursively process the nodes of any children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void Model::getVertices(aiMesh * mesh, const aiScene * scene, Mesh* finalMesh)
{
	vector<Vertex> vertices;

	vertices.resize(mesh->mNumVertices);

	// for each vertex of the mesh copy the data to out own mesh format
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// all meshes should have vertices and normals
		vertices[i].position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertices[i].normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		// check if the mesh has texture coordinates
		if (mesh->mTextureCoords[0])
		{
			vertices[i].textureCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertices[i].textureCoords = glm::vec2(0.0f, 0.0f);
		}
	}

	finalMesh->vertices = vertices;
}

void Model::getIndices(aiMesh * mesh, const aiScene * scene, Mesh* finalMesh)
{
	vector<unsigned int> indices;
	indices.resize(mesh->mNumFaces * 3); // imported as triangles

	// save all the vertex indices in the indices vector
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices[3 * i + j] = mesh->mFaces[i].mIndices[j];
	}
	
	finalMesh->indices = indices;
}

vector<Texture> Model::getTexture(aiMesh* mesh, const aiScene* scene)
{
	vector<Texture> textures;

	// get material textures (if there are any)
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");

		// put all the textures together in a single vector
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return textures;
}

void Model::render(const unsigned shaderProgram)
{
	for (size_t i = 0; i < v_meshes.size(); i++)
	{
		Mesh * mesh = &v_meshes[i];

		if (mesh->isMeshSetup)
		{
			mesh->render(shaderProgram);
		}
		else
		{
			if (mesh->vertices.size() != 0 && mesh->indices.size() != 0)
			{
				mesh->setupMesh();
			}
		}
	}
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool b_loadedTexture = false;
		for (auto texture : v_textures)
		{
			if (std::strcmp(texture.filepath.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(texture);
				b_loadedTexture = true;
				break;
			}
		}
		if (!b_loadedTexture)
		{   
			// setup a new texture
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.filepath = str;
			textures.push_back(texture);
			v_textures.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}


// static function to load a texture using lightweight stb_image library
unsigned int Model::TextureFromFile(const char* filepath, const string& directory, bool gamma)
{
	string filename = string(filepath);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	int width, height, nrComponents;
	unsigned char* textureData = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (textureData)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(textureData);
	}
	else
	{
		std::cout << "Texture failed to load from: " << filepath << std::endl;
		stbi_image_free(textureData);
	}

	return textureID;
}