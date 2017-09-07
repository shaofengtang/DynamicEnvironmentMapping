#include "TextRender.h"
#include "shader.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

/* CharacterSet Class Implementation */
CharacterSet::CharacterSet(const char* font)
{
	// Store font name
	std::string strFont(font);
	m_fontName = strFont.substr(strFont.find_last_of('/') + 1, strFont.find_last_of('.') - strFont.find_last_of('/') - 1);

	// Initialize FreeType Library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE:: Could not init FreeType Library\n";

	// Load the font as a face
	FT_Face face;
	if (FT_New_Face(ft, font, 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font\n";

	// Set font size
	FT_Set_Pixel_Sizes(face, 0, 48);

	init(face);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}


void CharacterSet::init(FT_Face& face)
{
	// Disable byte-alignment restriction
	// For each glyph, it has any width, and we cannot guarantee that the size is always a multiple of 4 bytes
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; ++c)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyoh\n";
			continue;
		}
		// Generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		//glGenerateMipmap(GL_TEXTURE_2D);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Store character for later use
		Character character;
		character.sTextureID = texture;
		character.sSize = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		character.sBearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		character.sAdvance = face->glyph->advance.x;

		m_characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

/* TextRender Class Implementation */
TextRender::TextRender()
{
	m_activeFontIdx = -1;
	init();
}

void TextRender::LoadFont(const char* font)
{
	CharacterSet newCharSet(font);
	m_fontsSet.push_back(newCharSet);

	if (m_fontsSet.size() == 1)
		m_activeFontIdx = 0;
}

void TextRender::ActivateFont(const char* fontName)
{
	if (m_fontsSet.size() == 0)
	{
		std::cout << "No fonts loaded. Activating font failed.\n";
		return;
	}

	std::string strFontName(fontName);

	for (int i = 0; i < m_fontsSet.size(); ++i)
	{
		if (m_fontsSet[i].m_fontName == strFontName)
		{
			m_activeFontIdx = i;
			return;
		}
	}

	// Not found
	std::cout << "Font: " << strFontName << "was not found.\n";
	if (m_activeFontIdx == -1)
		m_activeFontIdx = 0;

	return;
}

std::string TextRender::GetCurrentFontName()
{
	return m_fontsSet[m_activeFontIdx].m_fontName;
}

void TextRender::SetTextColor(float r, float g, float b)
{
	m_color.x = r;
	m_color.y = g;
	m_color.z = b;
}

void TextRender::init()
{
	m_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);

	// Initialize openGL rendering stuff
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// We render one character each time. We need one quad (two triangles) to show the character texture.
	// So we have 6 vertices and each vertex consists of vec4(x, y, u, v)
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRender::RenderText(Shader &shader, const char* text, GLfloat x, GLfloat y, GLfloat scale)
{
	// Check whether we have active font first
	if (m_activeFontIdx == -1 || m_fontsSet.size() == 0)
	{
		std::cout << "No fonts loaded. Text rendering failed.\n";
		return;
	}

	// Activate render state
	shader.use();
	shader.setVec3("textColor", m_color.x, m_color.y, m_color.z);
	shader.setMat4("projection", glm::value_ptr(m_projection));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	// Iterate all characters in the string
	std::string str(text);
	CharacterSet &charSet = m_fontsSet[m_activeFontIdx];

	for (int i = 0; i < str.length(); ++i)
	{
		Character ch = charSet.m_characters[str[i]];

		GLfloat xpos = x + ch.sBearing.x * scale;
		GLfloat ypos = y - (ch.sSize.y - ch.sBearing.y) * scale;

		GLfloat w = ch.sSize.x * scale;
		GLfloat h = ch.sSize.y * scale;

		GLfloat vertices[6][4] = 
		{
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.sTextureID);

		// Update VBO content
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render quad (character)
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.sAdvance >> 6) * scale;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}