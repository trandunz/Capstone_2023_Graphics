// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : TextLabel.h 
// Description : TextLabel Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Helper.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"

using Font = std::map<GLchar, FontChar>;

class TextLabel
{
public:
	/// <summary>
	/// TextLabel contructor.
	/// Requires a preloaded font.
	/// </summary>
	/// <param name="_windowSize"></param>
	/// <param name="_text"></param>
	/// <param name="_loadedFont"></param>
	/// <param name="_deltaTime"></param>
	/// <param name="_position"></param>
	/// <param name="_colour"></param>
	/// <param name="_scale"></param>
	TextLabel(glm::ivec2& _windowSize, std::string_view&& _text, Font& _loadedFont, float& _deltaTime, glm::vec2&& _position = { 0.0f,0.0f }, glm::vec4&& _colour = { 0.0f, 0.0f, 0.0f ,1.0f }, glm::vec2&& _scale = {1.0f,1.0f});
	
	/// <summary>
	/// TextLabel Destructor.
	/// </summary>
	~TextLabel();

	/// <summary>
	/// draws the text to the renderwindow
	/// </summary>
	void Draw();

	/// <summary>
	/// Returns the scale of the text
	/// </summary>
	/// <returns></returns>
	glm::vec2 GetScale();
	/// <summary>
	/// Returns the position of the text
	/// </summary>
	/// <returns></returns>
	glm::vec2 GetPosition();
	/// <summary>
	/// Reurns the right clip plane of the text
	/// </summary>
	/// <returns></returns>
	float GetRightClip();
	/// <summary>
	/// Returns the boundary extents of the text
	/// </summary>
	/// <returns></returns>
	glm::vec4 GetBounds();
	/// <summary>
	/// Returns the size of the text
	/// </summary>
	/// <returns></returns>
	glm::vec2 GetTextSize();
	/// <summary>
	/// Returns true if the text boundaries contain the position
	/// </summary>
	/// <param name="_position"></param>
	/// <returns></returns>
	bool BoundsContain(glm::vec2 _position);
	/// <summary>
	/// Sets the text
	/// </summary>
	/// <param name="_newText"></param>
	void SetText(std::string_view _newText);
	/// <summary>
	/// Sets the colour of the text
	/// </summary>
	/// <param name="_newColour"></param>
	void SetColour(glm::vec4&& _newColour);
	/// <summary>
	/// Sets the scale of the text
	/// </summary>
	/// <param name="_newScale"></param>
	void SetScale(glm::vec2&& _newScale);
	/// <summary>
	/// Sets the position of the text
	/// </summary>
	/// <param name="_newPosition"></param>
	void SetPosition(glm::vec2&& _newPosition);

	void SetScrollSpeed(float&& _newSpeed);
	void SetScrollingRight(bool&& _isScrollingRight);
	void SetScrolling(bool&& _isScrolling);
	/// <summary>
	/// Sets the clipping planes of the text (with respect to window size)
	/// </summary>s
	/// <param name="_leftClip"></param>
	/// <param name="_rightClip"></param>
	void SetClip(float&& _leftClip, float&& _rightClip);
	/// <summary>
	/// Sets the alignment of the text (e.g centre, left,right)
	/// </summary>
	/// <param name="_newAlignment"></param>
	void SetAlignment(TEXTALIGNMENT _newAlignment);
	/// <summary>
	/// Returns the current allignment of the text
	/// </summary>
	/// <returns></returns>
	TEXTALIGNMENT GetAlignment();
private:
	/// <summary>
	/// Updates the xpos and yPos values based on the alignment options
	/// </summary>
	/// <param name="_xPos"></param>
	/// <param name="_yPos"></param>
	/// <param name="_origin"></param>
	/// <param name="_fontCharacter"></param>
	void ApplyAlignment(GLfloat& _xPos, GLfloat& _yPos, glm::vec2& _origin, FontChar& _fontCharacter);

	bool m_FontLoaded = false;
	bool m_IsScrolling = false;
	bool m_ScrollRight = true;
	float* m_DeltaTime = nullptr;
	float m_ScrollSpeed = 100.0f;
	float m_RightClip = 250.0f;
	float m_LeftClip = 250.0f;

	std::string m_Label{};
	glm::vec2 m_Scale{ 1.0f, 1.0f };
	glm::vec2 m_Position{ 0.0f, 0.0f };
	glm::vec4 m_Colour{ 1.0f,1.0f,1.0f,1.0f };

	TEXTALIGNMENT m_Alignment = TEXTALIGNMENT::MIDDLE;

	GLuint m_VertexArrayID;
	GLuint m_VertexBufferID;
	GLuint m_IndexBufferID;
	GLuint m_ProgramID;
	glm::mat4 m_ProjectionMatrix;
	glm::ivec2* m_WindowSize = nullptr;
	Font* m_Font = nullptr;
};

