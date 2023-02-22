// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : FontLoader.cpp 
// Description : FontLoader Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "FontLoader.h"

Font FontLoader::LoadFont(std::string&& _fileName, unsigned _characterLimit)
{
	FT_Library fontLibrary;
	FT_Face fontFace;
	Font returnFont{};
	_fileName = "Resources/Fonts/" + _fileName;

	// Initalize fontLibary
	if (FT_Init_FreeType(&fontLibrary) != 0)
	{
		Print("Failed To Inialize FreeType Library");
		return returnFont;
	}
	// Load the font from file
	if (FT_New_Face(fontLibrary, _fileName.data(), 0, &fontFace) != 0)
	{
		Print("Failed To Load Font");
		return returnFont;
	}

	// TargetFace, nominal width, norminal height
	FT_Set_Pixel_Sizes(fontFace, 0, 100);
	// Set pixel storage mode for unpacking texture patterns
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Foreach glyph upto the character limit
	for (GLubyte glyph = 0; glyph < _characterLimit; glyph++)
	{
		// Load the glyph from the font face specifying load flag FT_LOAD_RENDER
		if (FT_Load_Char(fontFace, glyph, FT_LOAD_RENDER))
		{
			// If glyph failed, continue to next one
			std::string message = "Failed To Load Freetype Glyph: ";
			message += glyph;
			Print(message);
			continue;
		}

		// Create fontChar object, assigning textureID, size, bearing and advance
		FontChar fontCharacter =
		{
			LoadFontTexture(std::move(fontFace)), // Texture
			{fontFace->glyph->bitmap.width,fontFace->glyph->bitmap.rows }, // Set Size
			{fontFace->glyph->bitmap_left,fontFace->glyph->bitmap_top }, // Set Bearing
			(GLuint)fontFace->glyph->advance.x / 64 // Set Advance
		};
		// Add the glyph as a key and loaded fontCharacter to the font map for return
		returnFont.emplace(std::make_pair(glyph, fontCharacter));
	}

	// Cleanup the fontFace and Libary
	FT_Done_Face(fontFace);
	FT_Done_FreeType(fontLibrary);

	// Return the font map ready for use
	return returnFont;
}

GLuint FontLoader::LoadFontTexture(FT_Face&& _fontFace)
{
	// Generate a texture for the fontFace glyph
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RED,
		_fontFace->glyph->bitmap.width,
		_fontFace->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		_fontFace->glyph->bitmap.buffer);

	// Set its texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Return the textureID
	return textureID;
}
