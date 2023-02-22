// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Helper.h 
// Description : Helper Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <functional>

/// <summary>
/// Alias For Keymap (int = Key, bool = bPressed)
/// </summary>
using KEYMAP = std::map<int, bool>;

/// <summary>
/// ShaderProgramLocation struct that encapuslates the file paths of a programs shaders.
/// </summary>
struct ShaderProgramLocation
{
    const char* vertShader;
    const char* fragShader;
};

/// <summary>
/// UniformLocation struct that encapsulates the uniforms program and location.
/// </summary>
struct UniformLocation
{
    GLuint program;
    const char* location;
};

/// <summary>
/// Vertex struct that encapsulates Positional data, textureCoordinates and Normals
/// </summary>
struct Vertex
{
	glm::vec3 position{0,0,0};
	glm::vec2 texCoords{0,0};
	glm::vec3 normals{ 0,0,0 };
};

/// <summary>
/// Transform struct that encapsulates positional data such as translation, rotation and scale.
/// </summary>
struct Transform
{
    glm::mat4 transform = glm::mat4(1);
    glm::vec3 rotation_axis = { 0,0,0 };
    glm::vec3 translation = {0,0,0};
    glm::vec3 scale = {1,1,1};
    GLfloat rotation_value = 0.0f;
};

/// <summary>
/// Texture Struct that contains the ID of the texture, 
/// Its dimentions and its filePath.
/// </summary>
struct Texture
{
	GLuint ID = 0;
	glm::vec2 Dimensions{ 0 };
	std::string FilePath = "";
};

/// <summary>
/// Shape Enum To Identify Shapes For Mesh Class
/// </summary>
enum class SHAPE
{
	UNASSIGNED,
	CUBE,
	PYRAMID,
	SPHERE
};

/// <summary>
/// Font Character Struct that contains the characters texture id, size, bearing and advance
/// </summary>
struct FontChar
{
	GLuint textureID = 0; // Texture ID
	glm::ivec2 size{ 0,0 }; // Size of 'glyph'
	glm::ivec2 bearing{ 0,0 }; // Offset of 'glyph' from baseline
	GLuint advance = 0; // Distance To Next Character
};

/// <summary>
/// TEXTALIGNMENT enum to specify what sort of alignment TextLabels Should Use When Rendering
/// </summary>
enum class TEXTALIGNMENT
{
	LEFT,
	MIDDLE,
	RIGHT
};

/// <summary>
/// Updates the provided transform's model matrix with 
/// its positional, rotational and scaling values.
/// </summary>
/// <param name="_transform"></param>
/// <returns></returns>
inline glm::mat4& UpdateModelValueOfTransform(Transform& _transform)
{
	_transform.transform = glm::mat4(1);
	_transform.transform = glm::translate(_transform.transform, _transform.translation);
	if (_transform.rotation_axis.x > 0 ||
		_transform.rotation_axis.y > 0 ||
		_transform.rotation_axis.z > 0)
	{
		_transform.transform = glm::rotate(_transform.transform, _transform.rotation_value, _transform.rotation_axis);
	}
	_transform.transform = glm::scale(_transform.transform, _transform.scale);
	return _transform.transform;
}

/// <summary>
/// returns the value of pi at specified angle from sin(_angle * _xScale) / _yScale) + _offset
/// </summary>
/// <param name="_angle"></param>
/// <param name="_yScale"></param>
/// <param name="_xScale"></param>
/// <param name="_offset"></param>
/// <returns></returns>
inline float ClampedSin(float _angle, float _yScale, float _xScale, float _offset)
{
	return (sinf(_angle * _xScale) / _yScale) + _offset;
}

/// <summary>
/// Returns the Magnitude of the specified vector3
/// </summary>
/// <param name="_vector"></param>
/// <returns></returns>
inline float Magnitude(glm::vec3 _vector)
{
	return sqrtf((_vector.x * _vector.x) + (_vector.y * _vector.y) + (_vector.z * _vector.z));
}

/// <summary>
/// Prints the given string withought a heap allocation to the console output stream.
/// </summary>
/// <param name="_string"></param>
inline void Print(std::string_view _string)
{
	std::cout << _string << std::endl;
}

/// <summary>
/// Prints the Given Vector with format Vector X:~ | Y:~ | Z:~ 
/// </summary>
/// <param name="_vector"></param>
inline void Print(glm::vec3& _vector)
{
	std::cout << "Vector x:" << std::to_string(_vector.x) << "| Y: " << std::to_string(_vector.y) << "| Z:" << std::to_string(_vector.z) << std::endl;
}

/// <summary>
/// Prints the Given Vector with format Vector X:~ | Y:~ | Z:~ | W:~
/// </summary>
/// <param name="_vector"></param>
inline void Print(glm::vec4 _vector)
{
	std::cout << "Vector x:" << std::to_string(_vector.x) << "| Y: " << std::to_string(_vector.y) << "| Z:" << std::to_string(_vector.z) << "| W:" << std::to_string(_vector.w) << std::endl;
}

/// <summary>
/// Prints the given integer to the console output stream.
/// </summary>
/// <param name="_int"></param>
inline void Print(int&& _int)
{
	std::cout << _int << std::endl;
}

/// <summary>
/// Prints the given float to the console output Stream.
/// </summary>
/// <param name="_float"></param>
inline void Print(float&& _float)
{
	std::cout << _float << std::endl;
}