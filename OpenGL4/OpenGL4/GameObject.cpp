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
            if (key.first == GLFW_KEY_W)
            {
                m_Input.z -= 1.0f;
            }
            if (key.first == GLFW_KEY_A)
            {
                m_Input.x -= 1.0f;
            }
            if (key.first == GLFW_KEY_S)
            {
                m_Input.z += 1.0f;
            }
            if (key.first == GLFW_KEY_D)
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
        // Bind shader
        glUseProgram(m_ShaderID);

        // If shader program is single texture
        if (m_ShaderLocation.vertShader == "SingleTexture.vert" && 
            m_ShaderLocation.fragShader == "SingleTexture.frag")
        {
            SetSingleTextureUniforms();
        }
        // Else if Vertex shader is 3D with Normals
        else if (m_ShaderLocation.vertShader == "Normals3D.vert")   
        {
            SetNormals3DVertUniforms();
        
            // If Frag Shader is Blinn_Phong Lighting
            if (m_ShaderLocation.fragShader == "BlinnFong3D.frag")
            {
                SetBlinnFong3DUniforms();
                SetRimLighingUniforms();
            }
            // Else if Frag Shader is Reflection
            else if (m_ShaderLocation.fragShader == "Reflection.frag")
            {
                SetReflectionUniforms();
            }
            // Else If Frag Shader Is ReflectionMap
            else if (m_ShaderLocation.fragShader == "ReflectionMap.frag")
            {
                SetReflectionMapUniforms();
            }
            // Else If Frag Shader IS Blinn_Phong Lighting With Reflection
            else if (m_ShaderLocation.fragShader == "BlinnFong3D_Reflection.frag")
            {
                SetBlinnFong3DUniforms();
                SetRimLighingUniforms();
                SetReflectionMapUniforms();
            }
        }
        

        // Draw the mesh
        m_Mesh->Draw();

        // Unbind
        glUseProgram(0);
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

void GameObject::SetShader(const char* _vertexSource,const char* _fragmentSource)
{
    m_ShaderID = ShaderLoader::CreateShader(_vertexSource, _fragmentSource);
    m_ShaderLocation = { _vertexSource , _fragmentSource };
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

void GameObject::SetRimLighting(bool _rimLighting)
{
    m_RimLighting = _rimLighting;
}

void GameObject::SetBlinnFong3DUniforms()
{
    // Apply Texture
    if (m_ActiveTextures.size() > 0)
    {
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[0].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "ImageTexture0", 0);
    }

    // Set Global Ambient Colour And Strength
    ShaderLoader::SetUniform1f(std::move(m_ShaderID), "AmbientStrength", 0.5f);
    ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "AmbientColor", { 1.0f,1.0f,1.0f });

    // Set Shininess
    ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Shininess", 32.0f * 5);
    
    // Set Camera Position
    ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", m_ActiveCamera->GetPosition());

    if (m_LightManager)
    {
        // Set Point Light Uniforms From Light Manager
        std::vector<PointLight>& pointLights = m_LightManager->GetPointLights();
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfPointLights", (int)pointLights.size());
        for (unsigned i = 0; i < pointLights.size(); i++)
        {
            ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].Position", pointLights[i].Position);
            ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].Color", pointLights[i].Color);
            ShaderLoader::SetUniform1f(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].SpecularStrength", pointLights[i].SpecularStrength);
            ShaderLoader::SetUniform1f(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].AttenuationLinear", pointLights[i].AttenuationLinear);
            ShaderLoader::SetUniform1f(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].AttenuationExponent", pointLights[i].AttenuationExponent);
        }

        // Set Directional Light Uniforms From Light Manager
        std::vector<DirectionalLight>& directionalLights = m_LightManager->GetDirectionalLights();
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfDirectionalLights", (int)directionalLights.size());
        for (unsigned i = 0; i < directionalLights.size(); i++)
        {
            ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].Direction", directionalLights[i].Direction);
            ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].Color", directionalLights[i].Color);
            ShaderLoader::SetUniform1f(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].SpecularStrength", directionalLights[i].SpecularStrength);
        }

        // Set Spotlight Uniforms From Light Manager
        std::vector<SpotLight>& spotLights = m_LightManager->GetSpotLights();
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfSpotLights", (int)spotLights.size());
        for (unsigned i = 0; i < spotLights.size(); i++)
        {
            // If the spotlight is attached to the camera, Set Uniforms Accordingly
            if (spotLights[i].IsAttachedToCamera)
            {
                ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Position", m_ActiveCamera->GetPosition());
                ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Direction", m_ActiveCamera->GetFront());
            }
            // Else Apply Assigned Starting Positon And Direction
            else
            {
                ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Position", spotLights[i].Position);
                ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Direction", spotLights[i].Direction);
            }
            ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Color", spotLights[i].Color);
            ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].SpecularStrength", spotLights[i].SpecularStrength);
            ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].AttenuationLinear", spotLights[i].AttenuationLinear);
            ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].AttenuationExponent", spotLights[i].AttenuationExponent);
            ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Cutoff", glm::cos(glm::radians(spotLights[i].Cutoff)));
            ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].OuterCutoff", glm::cos(glm::radians(spotLights[i].OuterCutoff)));
        }
    }
}

void GameObject::SetRimLighingUniforms()
{
    // Set Rim Lighting Bool
    ShaderLoader::SetUniform1i(std::move(m_ShaderID), "bRimLighting", m_RimLighting);

    // If RimLighting Enabled Then Set Colour And Exponent
    if (m_RimLighting)
    {
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "RimExponent", 4.0f);
        ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "RimColor", { 1.0f,0.0f,0.0f });
    }
}

void GameObject::SetReflectionUniforms()
{
    // Set Camera Position
    ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", m_ActiveCamera->GetPosition());

    // Bind And Set Skybox Texture Uniform
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTexture.ID);
    ShaderLoader::SetUniform1i(std::move(m_ShaderID), "SkyboxTexture", 0);
}

void GameObject::SetReflectionMapUniforms()
{
    // If two or more textures are present
    if (m_ActiveTextures.size() > 1)
    {
        // Set Regular Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[0].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
        // Set Reflection Map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[1].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "ReflectionMap", 1);
    }
    // Set Camera Pos
    ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", m_ActiveCamera->GetPosition());

    // Bind And Set Skybox Texture Uniform
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTexture.ID);
    ShaderLoader::SetUniform1i(std::move(m_ShaderID), "SkyboxTexture", 2);
}

void GameObject::SetNormals3DVertUniforms()
{
    // Projection * View * Model Matrix
    ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", m_ActiveCamera->GetPVMatrix() * m_Transform.transform);

    // Set Model Matrix
    ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "ModelMatrix", m_Transform.transform);
}

void GameObject::SetSingleTextureUniforms()
{
    // Apply Texture
    if (m_ActiveTextures.size() > 0)
    {
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[0].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
    }

    // Projection * View * Model Matrix
    ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", m_ActiveCamera->GetPVMatrix() * m_Transform.transform);
}


