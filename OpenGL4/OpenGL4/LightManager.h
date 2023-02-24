// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : LightManager.h 
// Description : LightManager Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

/// <summary>
/// Struct For A Point Light.
/// 1: Position, Default: 0,0,0
/// 2: Color, Default: 1,1,1
/// 3: Specular Strength, Default: 1.0f
/// 4: Linear Attenuation, Default: 0.045f
/// 5: Exponent Attenuation, Default: 0.0075f
/// </summary>
struct PointLight
{
    glm::vec3 Position{0,0,0};
    glm::vec3 Color{1.0f,0.0f,0.0f};
    float SpecularStrength = 1.0f;

    float AttenuationLinear = 0.045f;
    float AttenuationExponent = 0.0075f;
};

/// <summary>
/// Struct for a Directional Light
/// 1: Direction, Default: 0,-1,0
/// 2: Color, Default: 1,1,1
/// 3: Specular Strength, Default: 1.0f
/// </summary>
struct DirectionalLight
{
    glm::vec3 Direction{0,1,0};
    glm::vec3 Color{ 1.0f,1.0f,1.0f };
    float SpecularStrength = 1.0f;
};

/// <summary>
/// Struct For A Spotlight.
/// 1: IsAttachedToCamera, Default: false
/// 2: Position, Default: 0,0,0
/// 3: Color, Default: 1,1,1
/// 4: Specular Strength, Default: 1.0f
/// 5: Direction, Default: 0,0,-1
/// 6: Linear Attenuation, Default: 0.045f
/// 7: Exponent Attenuation, Default: 0.0075f
/// 8: Inner Cutoff, Default : 5 degrees
/// 9: Outer Cutoff, Default : 10 degrees
/// </summary>
struct SpotLight
{
    bool IsAttachedToCamera{ false };

    glm::vec3 Position{ 0.0f,0.0f,0.0f };
    glm::vec3 Color{ 1.0f,1.0f,1.0f };
    float SpecularStrength = 1.0f;
    glm::vec3 Direction{ 0.0f,0.0f,-1.0f };

    float AttenuationLinear = 0.045f;
    float AttenuationExponent = 0.0075f;

    float Cutoff = 5.0f;
    float OuterCutoff = 10.0f;
};

class LightManager
{
public:

    /// <summary>
    /// Light Manager Contructor
    /// </summary>
    /// <param name="_activeCamera"></param>
    /// <param name="_maxPointLights"></param>
    /// <param name="_maxDirectionalLights"></param>
    /// <param name="_maxSpotLights"></param>
    LightManager(Camera& _activeCamera, int _maxPointLights = 1, int _maxDirectionalLights = 1, int _maxSpotLights = 1);
    
    /// <summary>
    /// Light Manager Destructor
    /// </summary>
    ~LightManager();

    /// <summary>
    /// Draws The Unlit PointLights
    /// </summary>
    void Draw();

    /// <summary>
    /// Set the mesh of the Unlit Pointlights
    /// </summary>
    /// <param name="_mesh"></param>
    void SetLightMesh(Mesh* _mesh);

    /// <summary>
    /// Create a point Light if current amount is less than max
    /// </summary>
    /// <param name="_newLight"></param>
    void CreatePointLight(PointLight _newLight);

    /// <summary>
    /// Create a directional light if current amount is less than max
    /// </summary>
    /// <param name="_newLight"></param>
    void CreateDirectionalLight(DirectionalLight _newLight);

    /// <summary>
    /// Create a spotlight if current amount is less than max
    /// </summary>
    /// <param name="_newLight"></param>
    void CreateSpotLight(SpotLight _newLight);

    /// <summary>
    /// Returns all pointlights
    /// </summary>
    /// <returns></returns>
    std::vector<PointLight>& GetPointLights();

    /// <summary>
    /// Returns all directional lights
    /// </summary>
    /// <returns></returns>
    std::vector<DirectionalLight>& GetDirectionalLights();

    /// <summary>
    /// Returns all spotlights
    /// </summary>
    /// <returns></returns>
    std::vector<SpotLight>& GetSpotLights();

    /// <summary>
    /// Set Number Of Max Point Lights Allowed
    /// Trims Excess If Needed.
    /// </summary>
    /// <param name="_maxAmount"></param>
    void SetMaxPointLights(int _maxAmount = 1);

    /// <summary>
    /// Set Number Of Max Directional Lights Allowed
    /// Trims Excess If Needed.
    /// </summary>
    /// <param name="_maxAmount"></param>
    void SetMaxDirectionalLights(int _maxAmount = 1);

    /// <summary>
    /// Set Number Of Max SpotLights Allowed.
    /// Trims Excess If Needed.
    /// </summary>
    /// <param name="_maxAmount"></param>
    void SetMaxSpotLights(int _maxAmount = 1);
private:

    Camera* m_ActiveCamera{ nullptr };
    GLuint m_UnlitMeshShaderID{ 0 };
    Mesh* m_LightMesh{nullptr};
    int m_MaxPointLights{1};
    int m_MaxDirectionalLights{ 1 };
    int m_MaxSpotLights{ 1 };
    std::vector<PointLight> m_PointLights{};
    std::vector<DirectionalLight> m_DirectionalLights{};
    std::vector<SpotLight> m_SpotLights{};
};

