#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <glm/glm.hpp>

class CShader
{
protected:
	unsigned int m_uiD;
	bool m_bUsesGeometryShader;

	void CompileErrors(unsigned int _uShader, const char* _pType);

public:
	static std::string GetFileContents(const char* _pFileName);

	CShader(const char* _pVertexFile, const char* _pFragmentFile, const char* _pGeometryFile = "");
	~CShader();

	operator int() const;
	explicit operator int* ();
	const unsigned int& GetID();
	void Activate();
	void Deactivate();

	void Uniform1f(const char* _pUniform, float _v0);
	void Uniform2f(const char* _pUniform, float _v0, float _v1);
	void Uniform3f(const char* _pUniform, float _v0, float _v1, float _v2);
	void Uniform3f(const char* _pUniform, glm::vec3 _v0);
	void Uniform4f(const char* _pUniform, float _v0, float _v1, float _v2, float _v3);
	void Uniform4f(const char* _pUniform, glm::vec4 _v0);
	void Uniform1i(const char* _pUniform, int _v0);
	void Uniform2i(const char* _pUniform, int _v0, int _v1);
	void Uniform3i(const char* _pUniform, int _v0, int _v1, int _v2);
	void Uniform4i(const char* _pUniform, int _v0, int _v1, int _v2, int _v3);
	void Uniform1ui(const char* _pUniform, unsigned int _v0);
	void Uniform2ui(const char* _pUniform, unsigned int _v0, unsigned int _v1);
	void Uniform3ui(const char* _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2);
	void Uniform4ui(const char* _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2, unsigned int _v3);
	void Uniform1fv(const char* _pUniform, int _iCount, float* _v);
	void Uniform2fv(const char* _pUniform, int _iCount, float* _v);
	void Uniform3fv(const char* _pUniform, int _iCount, float* _v);
	void Uniform4fv(const char* _pUniform, int _iCount, float* _v);
	void Uniform1iv(const char* _pUniform, int _iCount, int* _v);
	void Uniform2iv(const char* _pUniform, int _iCount, int* _v);
	void Uniform3iv(const char* _pUniform, int _iCount, int* _v);
	void Uniform4iv(const char* _pUniform, int _iCount, int* _v);
	void Uniform1uiv(const char* _pUniform, int _iCount, unsigned int* _v);
	void Uniform2uiv(const char* _pUniform, int _iCount, unsigned int* _v);
	void Uniform3uiv(const char* _pUniform, int _iCount, unsigned int* _v);
	void Uniform4uiv(const char* _pUniform, int _iCount, unsigned int* _v);
};