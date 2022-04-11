#include "Model.h"

CModel::CModel(const char* _pFile)
{
	std::string strText = CShader::GetFileContents(_pFile);
	m_JSON = json::parse(strText);

	m_pFile = _pFile;
	m_vData = GetData();
}

std::vector<unsigned char> CModel::GetData()
{
	std::string strBytesText;
	std::string strUri = m_JSON["buffers"][0]["uri"];

	std::string strFile = std::string(m_pFile);
	std::string strFileDirectory = strFile.substr(0, strFile.find_last_of('/') + 1);
	strBytesText = CShader::GetFileContents((strFileDirectory + strUri).c_str());

	std::vector<unsigned char> vData(strBytesText.begin(), strBytesText.end());
	return vData;
}

void CModel::Draw(CShader& _Shader, CCamera& _Camera)
{

}