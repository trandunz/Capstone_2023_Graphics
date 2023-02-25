#pragma once
#include "Shader.h"
class StaticShader
{
public:
	static std::map<const char*, Shader*> Shaders;
};

