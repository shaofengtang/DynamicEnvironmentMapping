#include <string>

#include "light.h"
#include "shader.h"

/* Light Class Implamentation */
Light::Light(glm::vec3 ambient/* = glm::vec3(0.1f, 0.1f, 0.1f)*/, glm::vec3 diffuse/* = glm::vec3(0.8f, 0.8f, 0.8f)*/, glm::vec3 specular/* = glm::vec3(1.0f, 1.0f, 1.0f)*/)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}

void Light::SetAmbient(float x, float y, float z)
{
	m_ambient.x = x;
	m_ambient.y = y;
	m_ambient.z = z;
}

void Light::SetDiffuse(float x, float y, float z)
{
	m_diffuse.x = x;
	m_diffuse.y = y;
	m_diffuse.z = z;
}

void Light::SetSpecular(float x, float y, float z)
{
	m_specular.x = x;
	m_specular.y = y;
	m_specular.z = z;
}

/* DirLight Class Implementation */
DirLight::DirLight(glm::vec3 direction/* = glm::vec3(0.0f, 0.0f, -1.0f)*/, glm::vec3 ambient/* = glm::vec3(0.1f, 0.1f, 0.1f)*/, glm::vec3 diffuse/* = glm::vec3(0.8f, 0.8f, 0.8f)*/, glm::vec3 specular/* = glm::vec3(1.0f, 1.0f, 1.0f)*/)
	: Light(ambient, diffuse, specular)
{
	m_direction = direction;
}

void DirLight::SetDirection(float x, float y, float z)
{
	m_direction.x = x;
	m_direction.y = y;
	m_direction.z = z;
}

void DirLight::SetupInShader(Shader& shader, const char* uniName)
{
	shader.use();
	std::string name(uniName);
	shader.setVec3(name + ".direction", m_direction.x, m_direction.y, m_direction.z);
	shader.setVec3(name + ".ambient", m_ambient.x, m_ambient.y, m_ambient.z);
	shader.setVec3(name + ".diffuse", m_diffuse.x, m_diffuse.y, m_diffuse.z);
	shader.setVec3(name + ".specular", m_specular.x, m_specular.y, m_specular.z);
}

/* PointLight Class Implementation */
PointLight::PointLight(glm::vec3 position, float constant/* = 1.0f*/, float linear/* = 0.09f*/, float quadratic/* = 0.0032f*/, glm::vec3 ambient/* = glm::vec3(0.1f, 0.1f, 0.1f)*/, glm::vec3 diffuse/* = glm::vec3(0.8f, 0.8f, 0.8f)*/, glm::vec3 specular/* = glm::vec3(1.0f, 1.0f, 1.0f)*/)
	: Light(ambient, diffuse, specular)
{
	m_position = position;
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}

void PointLight::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void PointLight::SetConstant(float constant)
{
	m_constant = constant;
}

void PointLight::SetLinear(float linear)
{
	m_linear = linear;
}

void PointLight::SetQuadratic(float quadratic)
{
	m_quadratic = quadratic;
}

void PointLight::SetupInShader(Shader& shader, const char* uniName)
{
	shader.use();
	std::string name(uniName);
	shader.setVec3(name + ".position", m_position.x, m_position.y, m_position.z);
	shader.setVec3(name + ".ambient", m_ambient.x, m_ambient.y, m_ambient.z);
	shader.setVec3(name + ".diffuse", m_diffuse.x, m_diffuse.y, m_diffuse.z);
	shader.setVec3(name + ".specular", m_specular.x, m_specular.y, m_specular.z);
	shader.setFloat(name + ".constant", m_constant);
	shader.setFloat(name + ".linear", m_linear);
	shader.setFloat(name + ".quadratic", m_quadratic);
}

/* SpotLight Class Implementation */
SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float constant/* = 1.0f*/, float linear/* = 0.09f*/, float quadratic/* = 0.0032f*/, float innerCutoff/* = 0.218166f*//*12.5 deg*/, float outerCutoff/* = 0.305433f*//*17.5 deg*/, glm::vec3 ambient/* = glm::vec3(0.1f, 0.1f, 0.1f)*/, glm::vec3 diffuse/* = glm::vec3(0.8f, 0.8f, 0.8f)*/, glm::vec3 specular/* = glm::vec3(1.0f, 1.0f, 1.0f)*/)
	: Light(ambient, diffuse, specular)
{
	m_position = position;
	m_direction = direction;
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
	m_innerCutoff = innerCutoff;
	m_outerCutoff = outerCutoff;
}

void SpotLight::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void SpotLight::SetDirection(float x, float y, float z)
{
	m_direction.x = x;
	m_direction.y = y;
	m_direction.z = z;
}

void SpotLight::SetConstant(float constant)
{
	m_constant = constant;
}

void SpotLight::SetLinear(float linear)
{
	m_linear = linear;
}

void SpotLight::SetQuadratic(float quadratic)
{
	m_quadratic = quadratic;
}

void SpotLight::SetInnerCutoff(float innerCutoff)
{
	m_innerCutoff = innerCutoff;
}

void SpotLight::SetOuterCutoff(float outerCutoff)
{
	m_outerCutoff = outerCutoff;
}

void SpotLight::SetupInShader(Shader& shader, const char* uniName)
{
	shader.use();
	std::string name(uniName);
	shader.setVec3(name + ".position", m_position.x, m_position.y, m_position.z);
	shader.setVec3(name + ".direction", m_direction.x, m_direction.y, m_direction.z);
	shader.setVec3(name + ".ambient", m_ambient.x, m_ambient.y, m_ambient.z);
	shader.setVec3(name + ".diffuse", m_diffuse.x, m_diffuse.y, m_diffuse.z);
	shader.setVec3(name + ".specular", m_specular.x, m_specular.y, m_specular.z);
	shader.setFloat(name + ".constant", m_constant);
	shader.setFloat(name + ".linear", m_linear);
	shader.setFloat(name + ".quadratic", m_quadratic);
	shader.setFloat(name + ".innerCutoff", m_innerCutoff);
	shader.setFloat(name + ".outerCutoff", m_outerCutoff);
}