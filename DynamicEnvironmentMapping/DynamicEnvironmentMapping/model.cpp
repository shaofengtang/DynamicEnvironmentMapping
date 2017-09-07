#include "model.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

// helper function
unsigned int TextureFromFile(const char* path, const std::string &directory)
{
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	//std::cout << "DEBUD INFO: " << filename << std::endl;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}



// Model Class Implementation
Model::Model(const char* path) : m_minX(10000.0f), m_minY(10000.0f), m_minZ(10000.0f), m_maxX(-10000.0f), m_maxY(-10000.0f), m_maxZ(-10000.0f)
{
	loadModel(path);
}

void Model::Draw(Shader shader)
{
	for (unsigned int i = 0; i < m_meshes.size(); ++i)
		m_meshes[i].Draw(shader);
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		glm::vec3 v;
		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].y;
		v.z = mesh->mVertices[i].z;
		vertex.sPosition = v;
		// calculate bounding box
		if (v.x < m_minX) m_minX = v.x;
		if (v.x >= m_maxX) m_maxX = v.x;
		if (v.y < m_minY) m_minY = v.y;
		if (v.y >= m_maxY) m_maxY = v.y;
		if (v.z < m_minZ) m_minZ = v.z;
		if (v.z >= m_maxZ) m_maxZ = v.z;

		if (mesh->mNormals)
		{
			v.x = mesh->mNormals[i].x;
			v.y = mesh->mNormals[i].y;
			v.z = mesh->mNormals[i].z;
			vertex.sNormal = v;
		}

		if (mesh->mTextureCoords[0]) // we only care about the first texCoord now(up to 8 texs)
		{
			glm::vec2 v2;
			v2.x = mesh->mTextureCoords[0][i].x;
			v2.y = mesh->mTextureCoords[0][i].y;
			vertex.sTexCoords = v2;
		}
		else
			vertex.sTexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		// check if loaded
		for (unsigned int j = 0; j < m_texturesLoaded.size(); ++j)
		{
			if (std::strcmp(m_texturesLoaded[j].sPath.c_str(), str.C_Str()) == 0)
			{
				textures.push_back(m_texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		// if a new texture, we need to load it
		if (!skip)
		{
			Texture texture;
			texture.sId = TextureFromFile(str.C_Str(), m_directory);
			texture.sType = typeName;
			texture.sPath = std::string(str.C_Str());
			textures.push_back(texture);
			m_texturesLoaded.push_back(texture);
		}
	}
	return textures;
}

void Model::setMaterial(int meshIdx, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	// Set all meshes to the same material
	if (meshIdx == -1)
	{
		for (int i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i].m_material.sAmbient = ambient;
			m_meshes[i].m_material.sDiffuse = diffuse;
			m_meshes[i].m_material.sSpecular = specular;
			m_meshes[i].m_material.sShininess = shininess;
		}
	}
	else if (meshIdx >= m_meshes.size())
	{
		std::cout << "ERROR::MODEL::setMaterial(): Mesh index out of range.\n";
		return;
	}
	else
	{
		m_meshes[meshIdx].m_material.sAmbient = ambient;
		m_meshes[meshIdx].m_material.sDiffuse = diffuse;
		m_meshes[meshIdx].m_material.sSpecular = specular;
		m_meshes[meshIdx].m_material.sShininess = shininess;
	}
}