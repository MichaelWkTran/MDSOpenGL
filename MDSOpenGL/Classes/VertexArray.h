#pragma once

#include <GLEW/glew.h>
#include "VertexBuffer.h"

class CVertexArray
{
public:
	GLuint m_GLuID;
	CVertexArray();

	void LinkAttribute
	(
		CVertexBuffer& VertexBuffer, 
		GLuint GluLayout, 
		GLuint _GLuNumComponents, 
		GLenum _GLeType, 
		GLsizeiptr _Stride, 
		void* _Offset
	);
	void Bind();
	void Unbind();
	void Delete();
};

