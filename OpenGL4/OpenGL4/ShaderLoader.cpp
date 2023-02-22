// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : ShaderLoader.cpp 
// Description : ShaderLoader Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "ShaderLoader.h"

ShaderLoader::~ShaderLoader()
{
    glUseProgram(0);
    for (auto& item : m_ShaderPrograms)
    {
        glDeleteProgram(item.second);
    }
    for (auto& item : m_Shaders)
    {
        glDeleteShader(item.second);
    }

    m_Shaders.clear();
    m_ShaderPrograms.clear();
}

GLuint ShaderLoader::CreateShader(std::string&& _vertexShader, std::string&& _fragmentShader)
{
    // Create A Default Program
    GLuint program = glCreateProgram();

    // Create Shaders And Store There ID's Locally
    GLuint vertShader = CompileShader(GL_VERTEX_SHADER, PassFileToString(_vertexShader));
    GLuint fragShader = CompileShader(GL_FRAGMENT_SHADER, PassFileToString(_fragmentShader));

    // Attach Shaders To Program
    if (IsDebug)
    {
        Print("Attaching Shaders");
    }
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);

    // Link And Validate
    if (IsDebug)
    {
        Print("Linking program");
    }
    glLinkProgram(program);
    glValidateProgram(program);

    // Debug Output With Error Specific Message
    int result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);
        std::string debugOutput = "Failed to Compile Shader Program ";
        if (message != 0)
            debugOutput += message;
        Print(debugOutput);
        glDeleteProgram(program);
        _freea(message);
        return result;
    }

    // Push The New Shader Program To Vector
    m_ShaderPrograms.push_back(std::make_pair(ShaderProgramLocation{ _vertexShader.c_str(), _fragmentShader.c_str() }, program));

    // Return Program ID
    return program;
}

void ShaderLoader::SetUniform1i(GLuint&& _program, std::string_view&& _location, GLint _value)
{
    glUniform1i(glGetUniformLocation(_program, _location.data()), _value);
}

void ShaderLoader::SetUniform1f(GLuint&& _program, std::string_view&& _location, GLfloat _value)
{
    glUniform1f(glGetUniformLocation(_program, _location.data()), _value);
}

void ShaderLoader::SetUniform2i(GLuint&& _program, std::string_view&& _location, GLint _value, GLint _value2)
{
    glUniform2i(glGetUniformLocation(_program, _location.data()), _value, _value2);
}

void ShaderLoader::SetUniform2f(GLuint&& _program, std::string_view&& _location, GLfloat _value, GLfloat _value2)
{
    glUniform2f(glGetUniformLocation(_program, _location.data()), _value, _value2);
}

void ShaderLoader::SetUniform3i(GLuint&& _program, std::string_view&& _location, GLint _value, GLint _value2, GLint _value3)
{
    glUniform3i(glGetUniformLocation(_program, _location.data()), _value, _value2, _value3);
}

void ShaderLoader::SetUniform3f(GLuint&& _program, std::string_view&& _location, GLfloat _value, GLfloat _value2, GLfloat _value3)
{
    glUniform3f(glGetUniformLocation(_program, _location.data()), _value, _value2, _value3);
}

void ShaderLoader::SetUniform3fv(GLuint&& _program, std::string_view&& _location, glm::vec3 _value)
{
    glUniform3fv(glGetUniformLocation(_program, _location.data()), 1, glm::value_ptr(_value));
}

void ShaderLoader::SetUniform3iv(GLuint&& _program, std::string_view&& _location, glm::ivec3 _value)
{
    glUniform3iv(glGetUniformLocation(_program, _location.data()), 1, glm::value_ptr(_value));
}

void ShaderLoader::SetUniform4fv(GLuint&& _program, std::string_view&& _location, glm::vec4 _value)
{
    glUniform4fv(glGetUniformLocation(_program, _location.data()), 1, glm::value_ptr(_value));
}

void ShaderLoader::SetUniformMatrix4fv(GLuint&& _program, std::string_view&& _location, glm::mat4 _value)
{
    glUniformMatrix4fv(glGetUniformLocation(_program, _location.data()), 1, GL_FALSE, glm::value_ptr(_value));
}

GLuint ShaderLoader::CompileShader(GLenum&& _type, std::string&& _source)
{
    // Check If there Is Already A Shader With The Same Specifications Created
    for (auto& item : m_Shaders)
    {
        if (item.first == _source)
        {
            if (IsDebug)
            {
                Print("Re-used Shader " + std::to_string(item.second) + "!");
            }
            return item.second;
        }
    }

    // Create A Shader Of The Specified Type
    GLuint shader = glCreateShader(_type);

    // Grab The Shader Information Assign It To The Newly Created Shader
    const GLchar* src = _source.data();
    glShaderSource(shader, 1, &src, nullptr);

    // Print Compiling Shader (Type Specific)
    if (IsDebug)
    {
        switch (_type)
        {
        case GL_VERTEX_SHADER:
        {
            Print("Compiling vertex shader.");
            break;
        }
        case GL_FRAGMENT_SHADER:
        {
            Print("Compiling fragment shader.");
            break;
        }
        case GL_GEOMETRY_SHADER:
        {
            Print("Compiling geometry shader.");
            break;
        }
        default:
            break;
        }
    }

    // Compile The Shader
    glCompileShader(shader);

    // Debug Output With Error Specific Message
    int result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);
        std::string debugOutput = "Failed to Compile Vertex Shader";
        if (message != 0)
            debugOutput += message;
        Print(debugOutput);
        glDeleteShader(shader);
        _freea(message);
        return result;
    }

    // Push New Shader Into Vector
    m_Shaders.push_back(std::make_pair(_source, shader));

    return shader;
}

std::string ShaderLoader::PassFileToString(const std::string& _fileName)
{
    // Container For File Information
    std::string content{ "" };

    // File Stream
    std::ifstream fileStream("Resources/Shaders/" + _fileName);

    // If File Dident Open
    if (!fileStream.is_open())
    {
        std::string debugOutput = "Could not read file ";
        debugOutput += _fileName;
        debugOutput += ". File does not exist.";
        Print(debugOutput);
        return content;
    }

    // Read File And Pass Contents To 'content' string
    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();

    return content;
}
