#pragma once

#include <GLEW/glew.h>
#include<stb/stb_image.h>
#include "Shader.h"

class CTexture
{
public:
	GLuint m_GLuID;
	const char* m_pType;
	GLuint m_GLuUnit;

	CTexture(const char* _pImage, const char* _pType, GLuint _GLuSlot, GLenum _GLeFormat, GLenum _GLePixelType);

	void TextureUnit(CShader& _Shader, const char* _pUniform, GLuint _GLuUnit);
	void Bind();
	void Unbind();
	void Delete();
};

