#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <FREETYPE/ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>
#include <iostream>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Texture.h"
#include "../ExternVariables.h"

class CTextLabel
{
private:
	struct stFontChar
	{
		CTexture* pTexture;
		glm::ivec2 v2iSize;
		glm::ivec2 v2iBearing;
		GLuint GLuAdvance;
	};

	static const int m_iFontCharacterLimit = 128;
	bool m_bInitialized = false;
	glm::mat4 m_mat4Projection;
	std::map<GLchar, stFontChar> m_mapCharacter;

	CVertexArray m_VertexArray;
	CVertexBuffer m_VertexBuffer;
	CElementBuffer m_ElementBuffer;

	void UpdateSize();

public:
	std::string m_strText;
	glm::vec2 m_v2Position;
	unsigned int m_uHAlign; //0 = Left, 1 = Centre, 2 = Right
	unsigned int m_uVAlign; //0 = Top, 1 = Middle, 2 = Bottom
	glm::vec2 m_v2Size;
	glm::vec2 m_v2Scale;
	glm::vec3 m_v3Colour;
	CShader* m_pShader;
	
	CTextLabel
	(
		std::string _strText,
		std::string _strFont,
		CShader* _pShader,
		glm::ivec2 _v2iPixelSize,
		glm::vec2 _v2Position
	);

	const glm::mat4& GetProjectionMatrix();
	void Draw();
};