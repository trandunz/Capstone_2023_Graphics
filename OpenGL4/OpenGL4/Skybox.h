// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Skybox.h 
// Description : Skybox Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Mesh.h"
#include "Camera.h"
class Skybox
{
public:
	/// <summary>
	/// Creates A Skybox Singleton or Returns It.
	/// </summary>
	/// <param name="_activeCamera"></param>
	/// <param name="_cubemapTexture"></param>
	/// <returns></returns>
	static Skybox& GetInstance(Camera* _activeCamera, Texture _cubemapTexture)
	{
		static Skybox instance(_activeCamera, _cubemapTexture);
		return instance;
	}

	/// <summary>
	/// Delete Copy Contructor
	/// </summary>
	/// <param name=""></param>
	Skybox(Skybox const&) = delete;
	/// <summary>
	/// Delete Assignment Operator
	/// </summary>
	/// <param name=""></param>
	void operator=(Skybox const&) = delete;

	/// <summary>
	/// Handles Drawing of the skybox
	/// </summary>
	void Draw();

	/// <summary>
	/// Sets the Cubemap of the skybox
	/// </summary>
	/// <param name="_cubemapTexture"></param>
	void SetTexture(Texture _cubemapTexture);

	/// <summary>
	/// Sets the active camera
	/// </summary>
	/// <param name="_camera"></param>
	void SetActiveCamera(Camera* _camera);

	/// <summary>
	/// Gets the Cubemap Texture ID
	/// </summary>
	/// <returns></returns>
	Texture GetTextureID();

	/// <summary>
	/// Sets the position of the Skybox
	/// </summary>
	/// <param name="_newPosition"></param>
	void SetTranslation(glm::vec3 _newPosition);
	/// <summary>
	/// Translates the position of the Skybox by an amount
	/// </summary>
	/// <param name="_translation"></param>
	void Translate(glm::vec3 _translation);

	/// <summary>
	/// Sets the rotation of the Skybox
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void SetRotation(glm::vec3 _axis, float _degrees);
	/// <summary>
	/// Rotates the Skybox by an amount
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void Rotate(glm::vec3 _axis, float _degrees);

	/// <summary>
	/// Sets the scale of the Skybox
	/// </summary>
	/// <param name="_newScale"></param>
	void SetScale(glm::vec3 _newScale);
	/// <summary>
	/// Scales the Skybox by amount
	/// </summary>
	/// <param name="_scaleFactor"></param>
	void Scale(glm::vec3 _scaleFactor);

private:
	/// <summary>
	/// Private Constructor
	/// </summary>
	/// <param name="_activeCamera"></param>
	/// <param name="_cubemapTexture"></param>
	Skybox(Camera* _activeCamera, Texture _cubemapTexture);
	/// <summary>
	/// Private Destructor
	/// </summary>
	~Skybox();

	/// <summary>
	/// Create An Inverted Cube VAO
	/// </summary>
	void CreateInvertedCubeVAO();

	Transform m_Transform{};
	GLuint m_ShaderID{ 0 };
	Camera* m_ActiveCamera{ nullptr };
	Texture m_CubemapTexture{};
	GLuint m_VertexArrayID{ 0 };
};

