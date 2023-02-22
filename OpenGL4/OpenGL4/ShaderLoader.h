// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : ShaderLoader.h 
// Description : ShaderLoader Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Helper.h"

class ShaderLoader
{
public:
    static const bool IsDebug = false;

    /// <summary>
    /// Cleans Up All Shader And Program Caches
    /// </summary>
    ~ShaderLoader();

    /// <summary>
    /// Creates A Given Shader Program With The Specified Vertex Shader And Fragment Shader Paths And Returns Its ID.
    /// </summary>
    /// <param name="_vertexShader"></param>
    /// <param name="_fragmentShader"></param>
    /// <returns></returns>
    static GLuint CreateShader(std::string&& _vertexShader, std::string&& _fragmentShader);

    /// <summary>
    /// Sets Uniform 1i At Location
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniform1i(GLuint&& _program, std::string_view&& _location, GLint _value);
    
    /// <summary>
    /// Sets Uniform 1f At Location
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniform1f(GLuint&& _program, std::string_view&& _location, GLfloat _value);
    
    /// <summary>
    /// Sets Uniform 2i At Location
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    /// <param name="_value2"></param>
    static void SetUniform2i(GLuint&& _program, std::string_view&& _location, GLint _value, GLint _value2);
    
    /// <summary>
    /// Sets Uniform 2f At Location
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    /// <param name="_value2"></param>
    static void SetUniform2f(GLuint&& _program, std::string_view&& _location, GLfloat _value, GLfloat _value2);
    
    /// <summary>
    /// Sets Uniform 3i At Location
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    /// <param name="_value2"></param>
    /// <param name="_value3"></param>
    static void SetUniform3i(GLuint&& _program, std::string_view&& _location, GLint _value, GLint _value2, GLint _value3);
    
    /// <summary>
    /// Sets Uniform 3f At Location
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    /// <param name="_value2"></param>
    /// <param name="_value3"></param>
    static void SetUniform3f(GLuint&& _program, std::string_view&& _location, GLfloat _value, GLfloat _value2, GLfloat _value3);
    
    /// <summary>
    /// Sets Uniform 3fv At Location
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniform3fv(GLuint&& _program, std::string_view&& _location, glm::vec3 _value);
    
    /// <summary>
    /// Sets Uniform 3iv At Location
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniform3iv(GLuint&& _program, std::string_view&& _location, glm::ivec3 _value);
    
    /// <summary>
    /// Sets Uniform 4fv At Location
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniform4fv(GLuint&& _program, std::string_view&& _location, glm::vec4 _value);

    /// <summary>
    /// Sets Uniform 4fv At Location
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniformMatrix4fv(GLuint&& _program, std::string_view&& _location, glm::mat4 _value);
private:
    /// <summary>
    /// Compiles A Shader Of A Given Type And Source And Returns Its ID.
    /// </summary>
    /// <param name="_type"></param>
    /// <param name="_source"></param>
    /// <returns></returns>
    static GLuint CompileShader(GLenum&& _type, std::string&& _source);

    /// <summary>
    /// Passes A File At The Given Adress Into A String And Returns It.
    /// </summary>
    /// <param name="_fileAddress"></param>
    /// <returns></returns>
    static std::string PassFileToString(const std::string& _fileName);

    inline static std::vector<std::pair<ShaderProgramLocation, GLuint>> m_ShaderPrograms;
    inline static std::vector<std::pair<std::string, GLuint>> m_Shaders;
};

