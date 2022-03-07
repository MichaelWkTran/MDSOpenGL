#pragma once

#include <GLEW/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string GetFileContents(const char* _pFileName);

class CShader
{
public:
	GLuint m_GLuID;
	CShader(const char* _pVertexFile, const char* _pFragmentFile);

	void Activate();
	void Delete();

private:
	void CompileErrors(unsigned int _uShader, const char* _pType);
};