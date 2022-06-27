#include "LightManager.h"
//#include <fstream>

glm::vec4 CLightManager::m_v4AmbientColour = glm::vec4(1.0f,1.0f,1.0f,0.2f);
std::vector<stInfinitePointLight> CLightManager::m_vInfinitePointLight;
std::vector<stPointLight> CLightManager::m_vPointLight;
std::vector<stDirectionalLight> CLightManager::m_vDirectionalLight;
std::vector<stSpotLight> CLightManager::m_vSpotLight;

/*static*/ const unsigned int CLightManager::TotalLights()
{
	return m_vInfinitePointLight.size() + m_vPointLight.size() + m_vDirectionalLight.size() + m_vSpotLight.size();
}

/*static*/ bool CLightManager::UpdateDiffuseShader(const char* _pShaderFile)
{
	//std::fstream FStream(_pShaderFile);
	//if (FStream.is_open()) return false;
	//
	//bool bExitLoop = false;
	//while (!FStream.eof() && !bExitLoop)
	//{
	//	std::string strLine;
	//	std::getline(FStream, strLine);
	//	
	//	if (strLine.find(""))
	//}
	//
	//FStream.close();
	return true;
}

/*static*/ bool CLightManager::UpdateDiffuseShaders(std::vector<const char*> _vShaderFiles)
{
	//for (auto pShaderFiles : _vShaderFiles)
	//{
	//	if (!UpdateDiffuseShader(pShaderFiles)) return false;
	//}

	return true;
}

/*static*/ void CLightManager::UpdateShaderUniforms(CShader* _pShader)
{
	_pShader->Activate();

	//Set the uniforms foreach light type
	for (int i = 0; i < (int)m_vInfinitePointLight.size(); i++)
	{
		_pShader->Uniform3f(("uni_InfinitePointLight[" + std::to_string(i) + "].v3LightPosition").c_str(), m_vInfinitePointLight[i].v3LightPosition);
		_pShader->Uniform4f(("uni_InfinitePointLight[" + std::to_string(i) + "].v4LightColour").c_str(), m_vInfinitePointLight[i].v4LightColour);
	}
	
	for (int i = 0; i < (int)m_vPointLight.size(); i++)
	{
		_pShader->Uniform3f(("uni_PointLight[" + std::to_string(i) + "].v3LightPosition").c_str(), m_vPointLight[i].v3LightPosition);
		_pShader->Uniform4f(("uni_PointLight[" + std::to_string(i) + "].v4LightColour").c_str(), m_vPointLight[i].v4LightColour);
		_pShader->Uniform1f(("uni_PointLight[" + std::to_string(i) + "].fAttenuationExponent").c_str(), m_vPointLight[i].fAttenuationExponent);
		_pShader->Uniform1f(("uni_PointLight[" + std::to_string(i) + "].fAttenuationLinear").c_str(), m_vPointLight[i].fAttenuationLinear);
		_pShader->Uniform1f(("uni_PointLight[" + std::to_string(i) + "].fAttenuationConstant").c_str(), m_vPointLight[i].fAttenuationConstant);
	}

	for (int i = 0; i < (int)m_vDirectionalLight.size(); i++)
	{
		_pShader->Uniform3f(("uni_DirectionalLight[" + std::to_string(i) + "].v3LightDirection").c_str(), m_vDirectionalLight[i].v3LightDirection);
		_pShader->Uniform4f(("uni_DirectionalLight[" + std::to_string(i) + "].v4LightColour").c_str(), m_vDirectionalLight[i].v4LightColour);
	}

	for (int i = 0; i < (int)m_vSpotLight.size(); i++)
	{
		_pShader->Uniform3f(("uni_SpotLight[" + std::to_string(i) + "].v3LightPosition").c_str(), m_vSpotLight[i].v3LightPosition);
		_pShader->Uniform3f(("uni_SpotLight[" + std::to_string(i) + "].v3LightDirection").c_str(), m_vSpotLight[i].v3LightDirection);
		_pShader->Uniform4f(("uni_SpotLight[" + std::to_string(i) + "].v4LightColour").c_str(), m_vSpotLight[i].v4LightColour);
		_pShader->Uniform1f(("uni_SpotLight[" + std::to_string(i) + "].fOuterCone").c_str(), m_vSpotLight[i].fOuterCone);
		_pShader->Uniform1f(("uni_SpotLight[" + std::to_string(i) + "].fInnerCone").c_str(), m_vSpotLight[i].fInnerCone);
	}
	
	//Check whether the type of light is used or not
	_pShader->Uniform1i("uni_bUsesInfinitePointLight", m_vInfinitePointLight.size() > 0);
	_pShader->Uniform1i("uni_bUsesPointLight", m_vPointLight.size() > 0);
	_pShader->Uniform1i("uni_bUsesDirectionalLight", m_vDirectionalLight.size() > 0);
	_pShader->Uniform1i("uni_bUsesSpotLight", m_vSpotLight.size() > 0);

	//Set uni_v4AmbientColour
	_pShader->Uniform4f("uni_v4AmbientColour", m_v4AmbientColour);

	_pShader->Deactivate();
}

/*static*/ void CLightManager::UpdateShaderUniforms(std::vector<CShader*> _vShaders)
{
	for (auto pShader : _vShaders)
	{
		UpdateShaderUniforms(pShader);
	}
}