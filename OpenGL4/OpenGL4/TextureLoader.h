// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : TextureLoader.h 
// Description : TextureLoader Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Helper.h"
class TextureLoader
{
public:
	/// <summary>
	/// Cleans Up All Loaded Textures
	/// </summary>
	~TextureLoader();

	/// <summary>
	/// Initializes The Texture Loader With Any Initial Textures And Sets Flip_On_Load True
	/// </summary>
	static void Init(std::vector<const char*>&& _textures = {});

	/// <summary>
	/// Creates A Texture And Returns Its ID, FilePath And Dimentions In A The Struct Texture Using Cache Optimization.
	/// </summary>
	/// <param name="_filePath"></param>
	/// <returns></returns>
	static Texture LoadTexture(std::string&& _fileName);

	/// <summary>
	/// Creates a cubemap from the given 6 textures and returns its id and filepath in the struct Texture using Cache Optimization
	/// </summary>
	/// <param name="_fileNames"></param>
	/// <returns></returns>
	static Texture LoadCubemap(std::vector<std::string> _fileNames);

private:
	inline static std::vector<Texture> m_Textures;
};

