#pragma once
#include "Helper.h"

struct ShaderInfo
{
	GLenum Type{};
	const char* Filename{};
	GLuint ID;
};

class Shader
{
public:
	Shader(std::vector<ShaderInfo> _shaders, std::function<void()> _uniformsFunction);

	void Bind();
	void UnBind();
	std::function<void()> UniformsFunction;
	GLuint ID;
private:
	void SetUniforms();
};

