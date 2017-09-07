#ifndef __MODEL_HEADER__
#define __MODEL_HEADER__

#include "mesh.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include <fbxsdk.h>

#include <set>

class Model
{
public:
	Model(const char* path);
	virtual void Draw(Shader shader);

//protected:
public:
	/* Model Data */
	std::vector<Mesh> m_meshes;
	std::string m_directory;

	std::vector<Texture> m_texturesLoaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	void setMaterial(int meshIdx, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

	/* Bounding Box */
	float m_minX, m_minY, m_minZ, m_maxX, m_maxY, m_maxZ;
};

#endif
