#ifndef __MESH_HEADER__
#define __MESH_HEADER__

#include "shader.h"

#include <vector>
#include <string>

#include <glm/glm.hpp>

// Only for mesh without texture
struct Material
{
	glm::vec3 sAmbient;
	glm::vec3 sDiffuse;
	glm::vec3 sSpecular;
	float sShininess;
};

struct Vertex
{
	glm::vec3 sPosition;
	glm::vec3 sNormal;
	glm::vec2 sTexCoords;
};

struct Texture
{
	unsigned int sId;
	std::string sType;
	std::string sPath;
};

// Mesh Definition
class Mesh
{
public:
	/* Mesh Data */
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	// Only used when the mesh doesn't have texture
	Material m_material;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();
	void Draw(Shader shader);

private:
	/* Render data */
	unsigned int m_VAO, m_VBO, m_EBO;

	void setupMesh();
};

// Skeleton Definition
/*struct Skeleton
{
	std::vector<unsigned int> sChildrenSkeletonsIndices;
	int sParentSkeletonIndex; // -1 == root
	glm::vec3 sGlobalTranslation;
	std::string sJointName;
};*/

#endif
