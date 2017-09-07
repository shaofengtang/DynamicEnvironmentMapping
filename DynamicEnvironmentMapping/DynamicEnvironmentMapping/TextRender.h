#ifndef __TEXTRENDER_HEADER__
#define __TEXTRENDER_HEADER__

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

class Shader;

struct Character
{
	GLuint sTextureID; // ID handle of the glyph texture
	glm::ivec2 sSize; // Size of glyph
	glm::ivec2 sBearing; // Offset from baseline to left/top of glyph
	GLuint sAdvance; // Offset to advance to next glyph
};

class CharacterSet
{
public:
	std::map<GLchar, Character> m_characters;
	std::string m_fontName;

	CharacterSet(const char* font);

private:
	void init(FT_Face& face); // generate a texture and store its relevent data into a Character struct
};

class TextRender
{
public:
	std::vector<CharacterSet> m_fontsSet;

	TextRender();

	void LoadFont(const char* font);
	void ActivateFont(const char* fontName);
	void SetTextColor(float r, float g, float b);
	std::string GetCurrentFontName();
	void RenderText(Shader &shader, const char* text, GLfloat x, GLfloat y, GLfloat scale);

private:
	int m_activeFontIdx;
	glm::mat4 m_projection;
	glm::vec3 m_color;

	unsigned int m_VAO, m_VBO;

	void init();
};

#endif
