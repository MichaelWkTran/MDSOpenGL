#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

void CShader::CompileErrors(unsigned int _uShader, const char* _pType)
{
	GLint GLiHasCompiled;
	char cInfoLog[1024];

	if (_pType != "PROGRAM")
	{
		glGetShaderiv(_uShader, GL_COMPILE_STATUS, &GLiHasCompiled);
		if (GLiHasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(_uShader, 1024, NULL, cInfoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << _pType << "\n" << cInfoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(_uShader, GL_LINK_STATUS, &GLiHasCompiled);
		if (GLiHasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(_uShader, 1024, NULL, cInfoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << _pType << "\n" << cInfoLog << std::endl;
		}
	}
}

/*static */std::string CShader::GetFileContents(const char* _pFileName)
{
	std::ifstream In(_pFileName, std::ios::binary);
	if (In)
	{
		std::string strContents;
		In.seekg(0, std::ios::end);
		strContents.resize((unsigned int)In.tellg());
		In.seekg(0, std::ios::beg);
		In.read(&strContents[0], strContents.size());
		In.close();
		return strContents;
	}
	throw errno;
}

CShader::CShader(const char* _pVertexFile, const char* _pFragmentFile, const char* _pGeometryFile/* = ""*/)
{
	//Set up Shaders
	std::string strVertexCode = GetFileContents(_pVertexFile); const char* pVertexSource = strVertexCode.c_str();
	unsigned int GLuVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(GLuVertexShader, 1, &pVertexSource, NULL);
	glCompileShader(GLuVertexShader);
	CompileErrors(GLuVertexShader, "VERTEX");

	std::string strFragmentCode = GetFileContents(_pFragmentFile); const char* pFragmentSource = strFragmentCode.c_str();
	unsigned int GLuFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(GLuFragmentShader, 1, &pFragmentSource, NULL);
	glCompileShader(GLuFragmentShader);
	CompileErrors(GLuFragmentShader, "FRAGMENT");

	unsigned int GLuGeometryShader = 0;
	if (_pGeometryFile == "")
	{
		m_bUsesGeometryShader = false;
	}
	else
	{
		m_bUsesGeometryShader = true;
		std::string strGeometryCode = GetFileContents(_pGeometryFile); const char* pGeometrySource = strGeometryCode.c_str();
		GLuGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(GLuGeometryShader, 1, &pGeometrySource, NULL);
		glCompileShader(GLuGeometryShader);
		CompileErrors(GLuGeometryShader, "GEOMETRY");
	}

	//Create and link to Program
	m_uiD = glCreateProgram();

	glAttachShader(m_uiD, GLuVertexShader);
	glAttachShader(m_uiD, GLuFragmentShader);
	if (m_bUsesGeometryShader) glAttachShader(m_uiD, GLuGeometryShader);

	glLinkProgram(m_uiD);
	CompileErrors(m_uiD, "PROGRAM");

	glDeleteShader(GLuVertexShader);
	glDeleteShader(GLuFragmentShader);
	if (m_bUsesGeometryShader) glDeleteShader(GLuGeometryShader);
}

CShader::~CShader()
{
	glDeleteProgram(m_uiD);
}

CShader::operator int() const
{
	return m_uiD;
}

CShader::operator int* ()
{
	return nullptr;
}

const unsigned int& CShader::GetID()
{
	return m_uiD;
}

void CShader::Activate()
{
	glUseProgram(m_uiD);
}

void CShader::Deactivate()
{
	glUseProgram(0);
}

void CShader::Uniform1f(const char* _pUniform, float _v0)
{
	Activate();
	glUniform1f(glGetUniformLocation(m_uiD, _pUniform), _v0);
}

void CShader::Uniform2f(const char* _pUniform, float _v0, float _v1)
{
	Activate();
	glUniform2f(glGetUniformLocation(m_uiD, _pUniform), _v0, _v1);
}

void CShader::Uniform3f(const char* _pUniform, float _v0, float _v1, float _v2)
{
	Activate();
	glUniform3f(glGetUniformLocation(m_uiD, _pUniform), _v0, _v1, _v2);
}

void CShader::Uniform3f(const char* _pUniform, glm::vec3 _v0)
{
	Activate();
	glUniform3f(glGetUniformLocation(m_uiD, _pUniform), _v0.x, _v0.y, _v0.z);
}

void CShader::Uniform4f(const char* _pUniform, float _v0, float _v1, float _v2, float _v3)
{
	Activate();
	glUniform4f(glGetUniformLocation(m_uiD, _pUniform), _v0, _v1, _v2, _v3);
}

void CShader::Uniform4f(const char* _pUniform, glm::vec4 _v0)
{
	Activate();
	glUniform4f(glGetUniformLocation(m_uiD, _pUniform), _v0.x, _v0.y, _v0.z, _v0.w);
}

void CShader::Uniform1i(const char* _pUniform, int _v0)
{
	Activate();
	glUniform1i(glGetUniformLocation(m_uiD, _pUniform), _v0);
}

void CShader::Uniform2i(const char* _pUniform, int _v0, int _v1)
{
	Activate();
	glUniform2i(glGetUniformLocation(m_uiD, _pUniform), _v0, _v1);
}

void CShader::Uniform3i(const char* _pUniform, int _v0, int _v1, int _v2)
{
	Activate();
	glUniform3i(glGetUniformLocation(m_uiD, _pUniform), _v0, _v1, _v2);
}

void CShader::Uniform4i(const char* _pUniform, int _v0, int _v1, int _v2, int _v3)
{
	Activate();
	glUniform4i(glGetUniformLocation(m_uiD, _pUniform), _v0, _v1, _v2, _v3);
}

void CShader::Uniform1ui(const char* _pUniform, unsigned int _v0)
{
	Activate();
	glUniform1ui(glGetUniformLocation(m_uiD, _pUniform), _v0);
}

void CShader::Uniform2ui(const char* _pUniform, unsigned int _v0, unsigned int _v1)
{
	Activate();
	glUniform2ui(glGetUniformLocation(m_uiD, _pUniform), _v0, _v1);
}

void CShader::Uniform3ui(const char* _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2)
{
	Activate();
	glUniform3ui(glGetUniformLocation(m_uiD, _pUniform), _v0, _v1, _v2);
}

void CShader::Uniform4ui(const char* _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2, unsigned int _v3)
{
	Activate();
	glUniform4ui(glGetUniformLocation(m_uiD, _pUniform), _v0, _v1, _v2, _v3);
}