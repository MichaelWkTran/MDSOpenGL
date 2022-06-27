#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <map>
#include <vector>
#include "Shader.h"

class CTexture;

class CTextureManager
{
private:
	static std::map<unsigned int /*Texture ID*/, CTexture*> m_mapTextures;

public:
	static bool Empty();
	static unsigned int Size();
	static unsigned int MaxSize();

	CTexture* operator[](unsigned int _GLuID);
	static CTexture* At(unsigned int _GLuID);

	static CTexture* Insert(const char* _pType, unsigned int&& _GLuSlot, GLenum&& _GLeTarget = GL_TEXTURE_2D, const char* _pName = "");
	static CTexture* Insert(const char* _pImage, const char* _pType, unsigned int&& _GLuSlot, GLenum&& _GLeFormat, GLenum&& _GLePixelType, const char* _pName = "");
	static void Erase(unsigned int _GLuID);
	static void Clear();

	static void Bind(unsigned int _GLuID);
	static void Unbind();
};

class CTexture
{
private:
	friend CTextureManager;

	unsigned int m_GLuID;
	
	CTexture(const char* _pName, const char* _pType, GLenum&& _GLeTarget, unsigned int&& _GLuSlot);
	CTexture(CTexture const&) = delete;
	CTexture& operator=(const CTexture&) = delete;
	~CTexture();

public:
	const char* m_pName;
	const char* m_pType;
	GLenum m_GLeTarget;
	unsigned int m_GLuUnit;
	
	operator int() const;
	explicit operator int* ();
	const unsigned int GetID() const;
	void Uniform(unsigned int _GluShaderID, std::string _strUniformName);
	void Bind() const;
	static void Unbind();
};