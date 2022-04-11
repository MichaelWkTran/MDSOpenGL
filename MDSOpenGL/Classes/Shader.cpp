#include "Shader.h"

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

CShader::CShader(const char* _pVertexFile, const char* _pFragmentFile)
{
	std::string strVertexCode = GetFileContents(_pVertexFile);
	std::string strFragmentCode = GetFileContents(_pFragmentFile);

	const char* pVertexSource = strVertexCode.c_str();
	const char* pFragmentSource = strFragmentCode.c_str();

	//Set up Shaders
	GLuint GLuVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(GLuVertexShader, 1, &pVertexSource, NULL);
	glCompileShader(GLuVertexShader);
	CompileErrors(GLuVertexShader, "VERTEX");

	GLuint GLuFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(GLuFragmentShader, 1, &pFragmentSource, NULL);
	glCompileShader(GLuFragmentShader);
	CompileErrors(GLuFragmentShader, "FRAGMENT");

	m_GLuID = glCreateProgram();

	glAttachShader(m_GLuID, GLuVertexShader);
	glAttachShader(m_GLuID, GLuFragmentShader);

	glLinkProgram(m_GLuID);
	CompileErrors(m_GLuID, "PROGRAM");

	glDeleteShader(GLuVertexShader);
	glDeleteShader(GLuFragmentShader);
}

void CShader::Activate()
{
	glUseProgram(m_GLuID);
}

void CShader::Delete()
{
	glDeleteProgram(m_GLuID);
}

GLuint CShader::GetID()
{
	return m_GLuID;
}

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