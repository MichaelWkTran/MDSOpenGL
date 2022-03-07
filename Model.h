#pragma once

#include <json/json.h>
#include "Mesh.h"

using json = nlohmann::json;

class CModel
{
public:
	CModel(const char* _pFile);

	std::vector<unsigned char> GetData();
	void Draw(CShader& _Shader, CCamera& _Camera);

private:
	const char* m_pFile;
	std::vector<unsigned char> m_vData;
	json m_JSON;
};

