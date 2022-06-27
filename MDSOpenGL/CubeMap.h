#pragma once
#include "Classes/Mesh.h"

class CCubeMap
{
public:
	CCubeMap(CShader* _pShader, float _fSize, const char* _pTextureDirectories[6]);
	void Draw(const CCamera& _Camera);

private:
	CMesh m_Mesh;
};

