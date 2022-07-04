#pragma once
#include <vector>
#include "UpdatedObject.h"
#include "Mesh.h"

class CShader;

class CCubeSkybox : public CUpdatedObject
{
public:
	CCubeSkybox() {}
	CCubeSkybox(CShader* _pShader, float _fSize, const char* _pTextureDirectories[6]);
	~CCubeSkybox() {}

	void CreateSkybox(CShader* _pShader, float _fSize, const char* _pTextureDirectories[6]);
	void UpdateShaderUniforms(CShader* _pShader);
	void UpdateShaderUniforms(std::vector<CShader*> _vShaders);
	void Draw(const CCamera& _Camera);

private:
	CMesh<> m_Mesh;
};