#include "TextLabel.h"

void CTextLabel::UpdateSize()
{
	m_v2Size = glm::vec2();
	
	if (m_strText.empty()) return void();
	
	if (m_uHAlign != 0 || m_uVAlign < 2)
	{
		for (std::string::const_iterator TextCharacter = m_strText.begin(); TextCharacter != m_strText.end(); TextCharacter++)
		{
			stFontChar FontCharacter = m_mapCharacter[*TextCharacter];
			m_v2Size.x += FontCharacter.GLuAdvance;

			m_v2Size.y += (float)FontCharacter.v2iBearing.y;
		}

		m_v2Size.y /= (float)m_strText.size();

		m_v2Size.x *= m_v2Scale.x;
		m_v2Size.y *= m_v2Scale.y;
	}
}

CTextLabel::CTextLabel
(
	std::string _strText,
	std::string _strFont,
	CShader* _pShader,
	glm::ivec2 _v2iPixelSize,
	glm::vec2 _v2Position
)
{
	m_strText = _strText;
	m_pShader = _pShader;
	m_v2Position = _v2Position;
	m_uHAlign = 0;
	m_uVAlign = 0;
	m_v2Scale = glm::vec2(1.0f, 1.0f);
	m_v3Colour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_mat4Projection = glm::ortho(0.0f, (GLfloat)uViewPortW, 0.0f, (GLfloat)uViewPortH, 0.0f, 100.0f);
	
	FT_Library FontLibrary;
	FT_Face FontFace;

	//Initializing the Library object
	if (FT_Init_FreeType(&FontLibrary) != 0)
	{
		std::cout << "FreeType Error: Could not init FreeType Library";
		return;
	}
	//Loading the font as face
	if (FT_New_Face(FontLibrary, _strFont.c_str(), 0, &FontFace) != 0)
	{
		std::cout << "FreeType Error: Failed to load font";
		return;
	}

	FT_Set_Pixel_Sizes(FontFace, _v2iPixelSize.x, _v2iPixelSize.y);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Load the characters of the font up to the font character limit
	for (GLubyte Glyph = 0; Glyph < m_iFontCharacterLimit; Glyph++)
	{
		//Load the character glyph into face
		if (FT_Load_Char(FontFace, Glyph, FT_LOAD_RENDER))
		{
			std::cout << "FreeType Error: Failed to load Glyph: " << (unsigned char)Glyph << std::endl;
			continue;
		}

		auto GenerateTexture = [&]() -> CTexture*
		{
			CTexture* pTexture = CTextureManager::Insert("Glyph", 0);
			pTexture->Bind();

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, FontFace->glyph->bitmap.width, FontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, FontFace->glyph->bitmap.buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			return pTexture;
		};

		stFontChar FontCharacter =
		{
			GenerateTexture(),
			glm::ivec2(FontFace->glyph->bitmap.width,
			FontFace->glyph->bitmap.rows),
			glm::ivec2(FontFace->glyph->bitmap_left,
			FontFace->glyph->bitmap_top),
			(GLuint)FontFace->glyph->advance.x / 64
		};
		
		m_mapCharacter.insert(std::pair<GLchar, stFontChar>(Glyph, FontCharacter));
	}
	CTextureManager::Unbind();
	
	//Destroy FontLibrary and FontFace
	FT_Done_Face(FontFace);
	FT_Done_FreeType(FontLibrary);

	//Update m_v2Size
	UpdateSize();

	//Configure the VAO and VBO for texture quads
	std::vector<GLuint> vIndices =
	{
		0, 3, 1,
		0, 2, 3
	};
	m_ElementBuffer.SetIndicies(vIndices);

	m_VertexArray.Bind(); m_VertexBuffer.Bind(); m_ElementBuffer.Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL, GL_DYNAMIC_DRAW);
	
	m_VertexArray.LinkAttribute(0, 4, GL_FLOAT, 4 * sizeof(GLfloat), (void*)0);
	
	m_VertexBuffer.Unbind(); m_VertexArray.Unbind(); m_ElementBuffer.Unbind();

	m_bInitialized = true;
}

const glm::mat4& CTextLabel::GetProjectionMatrix()
{
	return m_mat4Projection;
}

void CTextLabel::Draw()
{
	if (!m_bInitialized || m_pShader == nullptr) return;

	//Bind VAO and Shader
	m_pShader->Activate(); m_VertexArray.Bind();
	
	//Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform3fv(glGetUniformLocation(m_pShader->GetID(), "uni_v3Color"), 1, glm::value_ptr(m_v3Colour));
	glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetID(), "uni_mat4Projection"), 1, GL_FALSE, glm::value_ptr(m_mat4Projection));

	// Update text width and height
	UpdateSize();
	
	// Store Character Origin that can change as we write each character. Keep the starting text position intact.
	glm::vec2 v2CharacterOrigin = m_v2Position; 
	switch (m_uHAlign)
	{
	case 0: break;
	case 1:
	{
		v2CharacterOrigin.x += -m_v2Size.x / 2.0f;
		break;
	}
	default:
	{
		v2CharacterOrigin.x += -m_v2Size.x;
	}
	}
	
	switch (m_uVAlign)
	{
	case 0:
	{
		v2CharacterOrigin.y += -m_v2Size.y;
		break;
	}
	case 1:
	{
		v2CharacterOrigin.y += -m_v2Size.y/2.0f; 
		break;
	}
	}

	// Iterate through the text characters and draw them
	for (std::string::const_iterator TextCharacter = m_strText.begin(); TextCharacter != m_strText.end(); TextCharacter++)
	{
		stFontChar FontCharacter = m_mapCharacter[*TextCharacter];
		
		GLfloat GLfPosX = v2CharacterOrigin.x + FontCharacter.v2iBearing.x * m_v2Scale.x;
		GLfloat GLfPosY = v2CharacterOrigin.y - (FontCharacter.v2iSize.y - FontCharacter.v2iBearing.y) * m_v2Scale.y;
		
		GLfloat GLfWidth = FontCharacter.v2iSize.x * m_v2Scale.x;
		GLfloat GLfHeight = FontCharacter.v2iSize.y * m_v2Scale.y;

		// Update VB0 for each character
		GLfloat GLuVertices[4][4] =
		{
			//Coordinates							   Texture Cordinate
			{ GLfPosX,            GLfPosY + GLfHeight, 0.0, 0.0 },
			{ GLfPosX + GLfWidth, GLfPosY + GLfHeight, 1.0, 0.0 },
			{ GLfPosX,            GLfPosY,             0.0, 1.0 },
			{ GLfPosX + GLfWidth, GLfPosY,             1.0, 1.0 }
		};
		
		// Reload the vertex array to the VB0
		m_VertexBuffer.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLuVertices), GLuVertices);

		// Render the glyph texture over the quad
		FontCharacter.pTexture->Bind();
		glUniform1i(glGetUniformLocation(m_pShader->GetID(), "uni_samp2DTextTexture"), 0);
		glDrawElements(GL_TRIANGLES, m_ElementBuffer.GetIndicies().size(), GL_UNSIGNED_INT, 0);

		// Move the position of the origin point for the next glyph by the advance
		v2CharacterOrigin.x += FontCharacter.GLuAdvance * m_v2Scale.x;
	}
	
	m_VertexBuffer.Unbind(); m_VertexArray.Unbind(); CTextureManager::Unbind(); m_pShader->Deactivate();
	glDisable(GL_BLEND);
}