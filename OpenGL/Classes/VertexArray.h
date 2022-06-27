#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"

class CVertexArray
{
private:
	GLuint m_GLuID;

public:
	CVertexArray();
	CVertexArray(CVertexArray const&) = delete;
	~CVertexArray();

	const GLuint& GetID();
	void LinkAttribute
	(
		GLuint GluLayout,
		GLuint _GLuNumComponents,
		GLenum _GLeType,
		GLsizeiptr _Stride,
		void* _Offset
	);
	void Bind();
	void Unbind();
};

