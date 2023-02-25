#include "Shader.h"
#include "ShaderLoader.h"

Shader::Shader(std::vector<ShaderInfo> _shaders, std::function<void()> _uniformsFunction)
{
    UniformsFunction = _uniformsFunction;

    // Create A Default Program
    ID = glCreateProgram();

    // Create Shaders And Store There ID's Locally
    for (auto& shader : _shaders)
    {
        shader.ID = ShaderLoader::CompileShader(shader.Type, ShaderLoader::PassFileToString(shader.Filename));
        glAttachShader(ID, shader.ID);
    }

    glLinkProgram(ID);
    glValidateProgram(ID);

    // Debug Output With Error Specific Message
    int result = 0;
    glGetProgramiv(ID, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length = 0;
        glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetProgramInfoLog(ID, length, &length, message);
        std::string debugOutput = "Failed to Compile Shader Program ";
        if (message != 0)
            debugOutput += message;
        Print(debugOutput);
        glDeleteProgram(ID);
        _freea(message);
    }

	
}

void Shader::Bind()
{
	glUseProgram(ID);
	SetUniforms();
}

void Shader::UnBind()
{
	glUseProgram(0);
}

void Shader::SetUniforms()
{
	if (UniformsFunction)
	{
		UniformsFunction();
	}
}
