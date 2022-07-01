#pragma once
#include "Mesh.h"

class CCubeSkybox
{
public:
	CCubeSkybox(CShader* _pShader, float _fSize, const char* _pTextureDirectories[6]);
	~CCubeSkybox() {}

	void UpdateShaderUniforms(CShader* _pShader);
	void UpdateShaderUniforms(std::vector<CShader*> _vShaders);
	void Draw(const CCamera& _Camera);

private:
	CMesh<> m_Mesh;
};