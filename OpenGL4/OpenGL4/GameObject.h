// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : GameObject.h 
// Description : GameObject Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "LightManager.h"

class GameObject
{
public:
	/// <summary>
	/// GameObject contructor
	/// </summary>
	/// <param name="_camera"></param>
	/// <param name="_deltaTime"></param>
	/// <param name="_position"></param>
	GameObject(Camera& _camera, glm::vec3 _position);
	
	/// <summary>
	/// GameObject Destructor
	/// </summary>
	~GameObject();

	/// <summary>
	/// Handles moving the object with keyboard.
	/// W: Forward
	/// S: Backward
	/// A: Left
	/// D: Right
	/// Q: Down
	/// E: Up
	/// </summary>
	/// <param name="_keypresses"></param>
	void Movement_WASDEQ(KEYMAP& _keymap);

	/// <summary>
	/// Update function for GameObject.
	/// Should be called every frame.
	/// </summary>
	void Update(float& _deltaTime);

	/// <summary>
	/// Draws The gameobject if it has a mesh attached
	/// </summary>
	void Draw();

	/// <summary>
	/// Attaches a mesh to be used for drawing.
	/// </summary>
	/// <param name="_mesh"></param>
	void SetMesh(Mesh* _mesh);
	/// <summary>
	/// Returns the attached mesh.
	/// </summary>
	/// <returns></returns>
	Mesh* GetMesh();

	/// <summary>
	/// Sets the position of the gameObject
	/// </summary>
	/// <param name="_newPosition"></param>
	void SetTranslation(glm::vec3 _newPosition);
	/// <summary>
	/// Translates the position of the gameObject by an amount
	/// </summary>
	/// <param name="_translation"></param>
	void Translate(glm::vec3 _translation);

	/// <summary>
	/// Sets the rotation of the gameObject
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void SetRotation(glm::vec3 _axis, float _degrees);
	/// <summary>
	/// Rotates the gameobject by an amount
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void Rotate(glm::vec3 _axis, float _degrees);

	/// <summary>
	/// Sets the scale of the gameobject
	/// </summary>
	/// <param name="_newScale"></param>
	void SetScale(glm::vec3 _newScale);
	/// <summary>
	/// Scales the gameObject by amount
	/// </summary>
	/// <param name="_scaleFactor"></param>
	void Scale(glm::vec3 _scaleFactor);

	/// <summary>
	/// Rotates the gameObject around the position on the given axis by specified degrees.
	/// </summary>
	/// <param name="_position"></param>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void RotateAround(glm::vec3&& _position, glm::vec3&& _axis, float&& _degrees);

	/// <summary>
	/// Sets the active camera of the gameObject
	/// </summary>
	/// <param name="_newCamera"></param>
	void SetActiveCamera(Camera& _newCamera);
	/// <summary>
	/// Returns the current active camera of the gameObject
	/// </summary>
	/// <returns></returns>
	Camera* GetActiveCamera();

	/// <summary>
	/// Sets the active textures to specified textures.
	/// </summary>
	/// <param name="_textures"></param>
	void SetActiveTextures(std::vector<Texture> _textures);
	/// <summary>
	/// Returns the vector of active textures
	/// </summary>
	/// <returns></returns>
	std::vector<Texture> GetActiveTextures();

	/// <summary>
	/// Sets the shader program to use for rendering
	/// </summary>
	/// <param name="_newShader"></param>
	void SetShader(const char* _vertexSource, const char* _fragmentSource);
	/// <summary>
	/// Returns the current shader program used for rendering
	/// </summary>
	/// <returns></returns>
	GLuint GetShader();

	/// <summary>
	/// Clears the input vector to prevent further movement
	/// </summary>
	void ClearInputVector();

	/// <summary>
	/// Sets The Light Manager Used Lighting
	/// </summary>
	/// <param name="_lightManager"></param>
	void SetLightManager(LightManager& _lightManager);

	/// <summary>
	/// Sets the texture of the skybox
	/// </summary>
	/// <param name="_skyboxTexture"></param>
	void SetSkyboxTexture(Texture _skyboxTexture);

	/// <summary>
	/// Toggle Rim Lighting If Blinn-Phong Shader Enabled
	/// </summary>
	/// <param name="_rimLighting"></param>
	void SetRimLighting(bool _rimLighting);
private:

	/// <summary>
	/// Handles Setting Blinn-Phong Shader Uniforms.
	/// Requres Blinn-Phong Shader Active
	/// </summary>
	void SetBlinnFong3DUniforms();

	/// <summary>
	/// Handles setting Rim Lighting Shader Uniforms (Combine With Blinn-Phong To Work)
	/// Requires Blinn-Phone Shader Active
	/// </summary>
	void SetRimLighingUniforms();

	/// <summary>
	/// Sets Reflection Specific Shader Uniforms
	/// </summary>
	void SetReflectionUniforms();

	/// <summary>
	/// Set Reflection Map Specific Shader Uniforms.
	/// Requires: Blinn-Phong_Reflection or Reflection Map Shaders
	/// </summary>
	void SetReflectionMapUniforms();

	/// <summary>
	/// Sets Uniforms for Use With Normals3D.vert
	/// </summary>
	void SetNormals3DVertUniforms();

	/// <summary>
	/// Set Uniforms For Single Texture
	/// </summary>
	void SetSingleTextureUniforms();

	bool m_RimLighting = false;
	std::vector<Texture> m_ActiveTextures{};
	GLuint m_ShaderID{0};
	ShaderProgramLocation m_ShaderLocation{nullptr,nullptr};
	glm::vec4 m_Input{};
	float m_MovementSpeed = 10.0f;
	Mesh* m_Mesh = nullptr;
	Camera* m_ActiveCamera = nullptr;
	LightManager* m_LightManager{ nullptr };
	Transform m_Transform{};
	Texture m_SkyboxTexture;
};

