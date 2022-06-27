#include "Texture.h"
#include <iostream>

#pragma region CTextureManager functions

/*static */std::map<unsigned int, CTexture*> CTextureManager::m_mapTextures;

/*static */bool CTextureManager::Empty()
{
	return m_mapTextures.empty();
}

/*static */unsigned int CTextureManager::Size()
{
	return m_mapTextures.size();
}

/*static */unsigned int CTextureManager::MaxSize()
{
	return m_mapTextures.max_size();
}

CTexture* CTextureManager::operator[](unsigned int _GLuID)
{
	return At(_GLuID);
}

/*static */CTexture* CTextureManager::At(unsigned int _GLuID)
{
	CTexture* pOutput = nullptr;
	
	try { pOutput = m_mapTextures.at(_GLuID); }
	catch (const std::out_of_range& oor) { std::cout << "OUT OF RANGE ERROR: " << oor.what() << '\n'; pOutput = nullptr; }
	
	return pOutput;
}

/*static */CTexture* CTextureManager::Insert(const char* _pType, unsigned int&& _GLuSlot, GLenum&& _GLeTarget/* = GL_TEXTURE_2D*/, const char* _pName/* = ""*/)
{
	CTexture* pTexture = new CTexture(_pName, _pType, std::move(_GLeTarget), std::move(_GLuSlot));
	m_mapTextures.insert(std::pair<unsigned int, CTexture*>(pTexture->GetID(), pTexture));

	return pTexture;
}

/*static */CTexture* CTextureManager::Insert(const char* _pImage, const char* _pType, unsigned int&& _GLuSlot, GLenum&& _GLeFormat, GLenum&& _GLePixelType, const char* _pName/* = ""*/)
{
	GLenum GLeFormat = _GLeFormat; GLenum GLePixelType = _GLePixelType;

	CTexture* pTexture = new CTexture(_pName, _pType, GL_TEXTURE_2D, std::move(_GLuSlot));
	m_mapTextures.insert(std::pair<unsigned int, CTexture*>(pTexture->GetID(), pTexture));

	int iImageWidth, iImageHeight, iImageComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pImageData = stbi_load(_pImage, &iImageWidth, &iImageHeight, &iImageComponents, 0);

	#pragma region Texture Bind
	pTexture->Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iImageWidth, iImageHeight, 0, GLeFormat, GLePixelType, pImageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(pImageData);

	Unbind();
	#pragma endregion

	return pTexture;
}

/*static */void CTextureManager::Erase(unsigned int _GLuID)
{
	auto Iterator = m_mapTextures.find(_GLuID);
	if (Iterator == m_mapTextures.end()) return void();

	delete (*Iterator).second;
	(*Iterator).second = nullptr;
	m_mapTextures.erase(Iterator);
}

/*static */void CTextureManager::Clear()
{
	for (auto& Texture : m_mapTextures)
	{
		if (Texture.second != nullptr) delete Texture.second;
		Texture.second = nullptr;
	}

	m_mapTextures.clear();
}

/*static */void CTextureManager::Bind(unsigned int _GLuID)
{
	At(_GLuID)->Bind();
}

/*static */void CTextureManager::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

#pragma endregion

#pragma region CTexture functions

CTexture::CTexture(const char* _pName, const char* _pType, GLenum&& _GLeTarget, unsigned int&& _GLuSlot)
{
	glGenTextures(1, &m_GLuID);
	m_pName = _pName;
	m_pType = _pType;
	m_GLeTarget = _GLeTarget;
	m_GLuUnit = _GLuSlot;

	_GLeTarget = 0;
	_GLuSlot = 0U;
}

CTexture::~CTexture()
{
	glDeleteTextures(1, &m_GLuID);
}

CTexture::operator int() const
{
	return m_GLuID;
}

CTexture::operator int* ()
{
	return nullptr;
}

const unsigned int CTexture::GetID() const
{
	return m_GLuID;
}

void CTexture::Uniform(unsigned int _GluShaderID, std::string _strUniformName)
{
	Bind();

	std::string strUniform = "uni_samp";
	
	if (m_GLeTarget == GL_TEXTURE_1D) strUniform += "1D";
	else if (m_GLeTarget == GL_TEXTURE_2D)  strUniform += "2D";
	else if (m_GLeTarget == GL_TEXTURE_3D)  strUniform += "3D";
	else if (m_GLeTarget == GL_TEXTURE_CUBE_MAP)  strUniform += "Cube";

	strUniform += _strUniformName;
	glUniform1i(glGetUniformLocation(_GluShaderID, strUniform.c_str()), m_GLuUnit + 1);
}

void CTexture::Bind() const
{
	glActiveTexture(GL_TEXTURE1 + m_GLuUnit);
	glBindTexture(m_GLeTarget, m_GLuID);
}

/*static */void CTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

#pragma endregion