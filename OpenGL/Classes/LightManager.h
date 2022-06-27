#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Camera.h"

struct stInfinitePointLight
{
	glm::vec3 v3LightPosition;
	glm::vec4 v4LightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

struct stPointLight
{
	glm::vec3 v3LightPosition;
	glm::vec4 v4LightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float fAttenuationExponent = 3.0f;
	float fAttenuationLinear = 0.7f;
	float fAttenuationConstant = 1.0f;
};

struct stDirectionalLight
{
	glm::vec3 v3LightDirection = glm::vec3(-1.0f, -1.0f, 1.0f);
	glm::vec4 v4LightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

struct stSpotLight
{
	glm::vec3 v3LightPosition;
	glm::vec3 v3LightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec4 v4LightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float fOuterCone = 0.9f;
	float fInnerCone = 0.95f;
};

class CLightManager
{
private:


public:
	static glm::vec4 m_v4AmbientColour;

	static std::vector<stInfinitePointLight> m_vInfinitePointLight;
	static std::vector<stPointLight> m_vPointLight;
	static std::vector<stDirectionalLight> m_vDirectionalLight;
	static std::vector<stSpotLight> m_vSpotLight;

	static const unsigned int TotalLights();
	static bool UpdateDiffuseShader(const char* _pShaderFile);
	static bool UpdateDiffuseShaders(std::vector<const char*> _vShaderFiles);
	static void UpdateShaderUniforms(CShader* _pShader);
	static void UpdateShaderUniforms(std::vector<CShader*> _vShaders);
};