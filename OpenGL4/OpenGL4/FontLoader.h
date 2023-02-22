// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : FontLoader.h 
// Description : FontLoader Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Helper.h"
#include "ft2build.h"
#include FT_FREETYPE_H

using Font = std::map<GLchar, FontChar>;

class FontLoader
{
public:
	/// <summary>
	/// Loads the given font and returns it.
	/// </summary>
	/// <param name="_filePath"></param>
	/// <param name="_characterLimit"></param>
	/// <returns></returns>
	static Font LoadFont(std::string&& _fileName, unsigned _characterLimit = 128);
private:
	/// <summary>
	/// Creates a texture for the given font face and returns its id.
	/// </summary>
	/// <param name="_fontFace"></param>
	/// <returns></returns>
	static GLuint LoadFontTexture(FT_Face&& _fontFace);
};

