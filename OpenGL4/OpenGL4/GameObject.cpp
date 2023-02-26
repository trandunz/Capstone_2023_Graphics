// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : GameObject.cpp 
// Description : GameObject Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "GameObject.h"

GameObject::GameObject(Camera& _camera, glm::vec3 _position)
{
    m_ActiveCamera = &_camera;
    // Set starting position
    SetTranslation(_position);

    StaticShader::Shaders["CellShading"]->UniformsFunction = [this]()
    {
        SetCellShadingUniforms();
    };
    StaticShader::Shaders["ToonOutline"]->UniformsFunction = [this]()
    {
        SetToonOutlineUniforms();
    };

}

GameObject::~GameObject()
{
    if (m_Mesh)
        m_Mesh = nullptr;

    if (m_ActiveCamera)
        m_ActiveCamera = nullptr;

    if (m_LightManager)
        m_LightManager = nullptr;

    m_ActiveTextures.clear();
}

void GameObject::Movement_WASDEQ(KEYMAP& _keymap)
{
    // Grab keyboard input for moving Object With WASDQE
    m_Input = {};

    for (auto& key : _keymap)
    {
        if (key.second)
        {
            if (key.first == GLFW_KEY_UP)
            {
                m_Input.z -= 1.0f;
            }
            if (key.first == GLFW_KEY_LEFT)
            {
                m_Input.x -= 1.0f;
            }
            if (key.first == GLFW_KEY_DOWN)
            {
                m_Input.z += 1.0f;
            }
            if (key.first == GLFW_KEY_RIGHT)
            {
                m_Input.x += 1.0f;
            }
            if (key.first == GLFW_KEY_Q)
            {
                m_Input.y -= 1.0f;
            }
            if (key.first == GLFW_KEY_E)
            {
                m_Input.y += 1.0f;
            }
            if (key.first == GLFW_KEY_Z)
            {
                m_Input.w -= 1.0f;
            }
            if (key.first == GLFW_KEY_C)
            {
                m_Input.w += 1.0f;
            }
        }
    }
    // Normalize the input vecor.
    glm::normalize(m_Input);
}

void GameObject::Update(float& _deltaTime)
{
    // If player provides input, Translate the gameobject accordingly.
    if (Magnitude((glm::vec3)m_Input) > 0)
        Translate(m_Input * _deltaTime * m_MovementSpeed);
    // If player provides Rotational input, rotate accordingly
    if (m_Input.w != 0)
        Rotate({ 0,1,0 }, m_Input.w * _deltaTime * 100);
}

void GameObject::Draw()
{
    if (m_Mesh)
    {

        //// Bind shader
        //for (auto& shader : m_Shaders)
        //{
        //    shader.Bind();
        //}
        
        //Bind normal Shader
        //Write to StencilBuffer
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        m_Shaders[0].Bind();
        // Draw the mesh
        m_Mesh->Draw();
        m_Shaders[0].UnBind();

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        //Bind Second Shader / Single Color Shader
        m_Shaders[1].Bind();

        m_Mesh->Draw();
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_DEPTH_TEST);
        m_Shaders[1].UnBind();

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}

void GameObject::SetMesh(Mesh* _mesh)
{
    m_Mesh = _mesh;
}

Mesh* GameObject::GetMesh()
{
    if (m_Mesh)
        return m_Mesh;
    else
        return nullptr;
}

void GameObject::SetTranslation(glm::vec3 _newPosition)
{
    m_Transform.translation = _newPosition;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::Translate(glm::vec3 _translation)
{
    m_Transform.translation += _translation;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::SetRotation(glm::vec3 _axis, float _degrees)
{
    m_Transform.rotation_axis = _axis;
    m_Transform.rotation_value = glm::radians(_degrees);
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::Rotate(glm::vec3 _axis, float _degrees)
{
    m_Transform.rotation_axis = _axis;
    m_Transform.rotation_value += glm::radians(_degrees);
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::SetScale(glm::vec3 _newScale)
{
    m_Transform.scale = _newScale;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::Scale(glm::vec3 _scaleFactor)
{
    m_Transform.scale *= _scaleFactor;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::RotateAround(glm::vec3&& _position, glm::vec3&& _axis, float&& _degrees)
{
    // get direction to the position
    glm::vec3 direction = glm::abs(_position - m_Transform.translation);
    // Translate to wards it
    m_Transform.transform = glm::translate(m_Transform.transform, direction);
    // Rotate
    m_Transform.transform = glm::rotate(m_Transform.transform, _degrees, _axis);
    // Translate back
    m_Transform.transform = glm::translate(m_Transform.transform, -direction);
}

void GameObject::SetActiveCamera(Camera& _newCamera)
{
    m_ActiveCamera = &_newCamera;
}

Camera* GameObject::GetActiveCamera()
{
    if (m_ActiveCamera)
        return m_ActiveCamera;
    else
        return nullptr;
}

void GameObject::SetActiveTextures(std::vector<Texture> _textures)
{
    m_ActiveTextures = _textures;
}

std::vector<Texture> GameObject::GetActiveTextures()
{
    return m_ActiveTextures;
}

void GameObject::SetShaders(std::vector<Shader> _shaders)
{
    m_Shaders = _shaders;
}

GLuint GameObject::GetShader()
{
    return m_ShaderID;
}

void GameObject::ClearInputVector()
{
    m_Input = {};
}

void GameObject::SetLightManager(LightManager& _lightManager)
{
    m_LightManager = &_lightManager;
}

void GameObject::SetSkyboxTexture(Texture _skyboxTexture)
{
    m_SkyboxTexture = _skyboxTexture;
}

void GameObject::SetToonOutlineUniforms()
{
    ShaderLoader::SetUniformMatrix4fv(std::move(StaticShader::Shaders["ToonOutline"]->ID), "PVMMatrix", m_ActiveCamera->GetPVMatrix() * m_Transform.transform);
    ShaderLoader::SetUniformMatrix4fv(std::move(StaticShader::Shaders["ToonOutline"]->ID), "ModelMatrix", m_Transform.transform);
    ShaderLoader::SetUniform1f(std::move(StaticShader::Shaders["ToonOutline"]->ID), "OutlineWidth", 0.2f);
    ShaderLoader::SetUniform3fv(std::move(StaticShader::Shaders["ToonOutline"]->ID), "Color", glm::vec4(0.0f,0.0f,0.0f,1.0f));
}

void GameObject::SetCellShadingUniforms()
{
    ShaderLoader::SetUniformMatrix4fv(std::move(StaticShader::Shaders["CellShading"]->ID), "PVMMatrix", m_ActiveCamera->GetPVMatrix() * m_Transform.transform);
    ShaderLoader::SetUniformMatrix4fv(std::move(StaticShader::Shaders["CellShading"]->ID), "ModelMatrix", m_Transform.transform);

    // Apply Texture
    if (m_ActiveTextures.size() > 0)
    {
        ShaderLoader::SetUniform1i(std::move(StaticShader::Shaders["CellShading"]->ID), "TextureCount", m_ActiveTextures.size());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[0].ID);
        ShaderLoader::SetUniform1i(std::move(StaticShader::Shaders["CellShading"]->ID), "ImageTexture0", 0);
    }

    // Set Global Ambient Colour And Strength
    ShaderLoader::SetUniform1f(std::move(StaticShader::Shaders["CellShading"]->ID), "AmbientStrength", 0.5f);
    ShaderLoader::SetUniform3fv(std::move(StaticShader::Shaders["CellShading"]->ID), "AmbientColor", { 1.0f,1.0f,1.0f });

    // Set Shininess
    ShaderLoader::SetUniform1f(std::move(StaticShader::Shaders["CellShading"]->ID), "Shininess", 32.0f * 5);

    // Set Camera Position
    ShaderLoader::SetUniform3fv(std::move(StaticShader::Shaders["CellShading"]->ID), "CameraPos", m_ActiveCamera->GetPosition());

    if (m_LightManager)
    {
        // Set Point Light Uniforms From Light Manager
        std::vector<PointLight>& pointLights = m_LightManager->GetPointLights();
        ShaderLoader::SetUniform1i(std::move(StaticShader::Shaders["CellShading"]->ID), "PointLightCount", (int)pointLights.size());
        for (unsigned i = 0; i < pointLights.size(); i++)
        {
            ShaderLoader::SetUniform3fv(std::move(StaticShader::Shaders["CellShading"]->ID), "PointLights[" + std::to_string(i) + "].Position", pointLights[i].Position);
            ShaderLoader::SetUniform3fv(std::move(StaticShader::Shaders["CellShading"]->ID), "PointLights[" + std::to_string(i) + "].Color", pointLights[i].Color);
            ShaderLoader::SetUniform1f(std::move(StaticShader::Shaders["CellShading"]->ID), "PointLights[" + std::to_string(i) + "].SpecularStrength", pointLights[i].SpecularStrength);
            ShaderLoader::SetUniform1f(std::move(StaticShader::Shaders["CellShading"]->ID), "PointLights[" + std::to_string(i) + "].AttenuationLinear", pointLights[i].AttenuationLinear);
            ShaderLoader::SetUniform1f(std::move(StaticShader::Shaders["CellShading"]->ID), "PointLights[" + std::to_string(i) + "].AttenuationExponent", pointLights[i].AttenuationExponent);
        }
    }
}