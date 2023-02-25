// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Mesh.cpp 
// Description : Mesh Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Mesh.h"
#include "TextureLoader.h"
#include "StaticShader.h"


Mesh::Mesh(SHAPE _shape, GLenum _windingOrder)
{
	m_WindingOrder = _windingOrder;

	CreateShapeVertices(_shape);
	CreateShapeIndices(_shape);
	CreateAndInitializeBuffers();
}

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned> _indices, std::vector<Texture> _textures)
{
	m_Vertices =_vertices;
	m_Indices = _indices;
	m_Textures = _textures;
	CreateAndInitializeBuffers();
}

Mesh::Mesh(std::string _modelName)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(("Resources/Models/" + _modelName).c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}


	ProcessNode(scene->mRootNode, scene);

	
	CreateAndInitializeBuffers();
}

Mesh::Mesh(unsigned int _numberOfSides, GLenum _windingOrder)
{
	m_WindingOrder = _windingOrder;

	CreatePolygonVertices(_numberOfSides);
	CreatePolygonIndices(_numberOfSides);
	CreateAndInitializeBuffers();
}

Mesh::~Mesh()
{
	m_Indices.clear();
	m_Vertices.clear();

	for (auto& mesh : m_Meshes)
	{
		delete mesh;
		mesh = nullptr;
	}

	// Unbind
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// Delete
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
		glDeleteBuffers(1, &m_VertexBufferID);
		glDeleteBuffers(1, &m_IndexBufferID);
	}
}

void Mesh::Draw()
{
	

	if (m_Meshes.size() > 0)
	{
		for (auto& mesh : m_Meshes)
		{
			for (int i = 0; i < m_Textures.size(); i++)
			{
				ShaderLoader::SetUniform1i(std::move(StaticShader::Shaders["CellShading"]->ID), "TextureCount", 0);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_Textures[i].ID);
				ShaderLoader::SetUniform1i(std::move(StaticShader::Shaders["CellShading"]->ID), "ImageTexture" + std::to_string(i), i);
			}
			mesh->Draw();
		}
	}
	else
	{
		glBindVertexArray(m_VertexArrayID);
		glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
		

	
}

void Mesh::CreateShapeVertices(SHAPE _shape)
{
	switch (_shape)
	{
	case SHAPE::CUBE:
	{
		// Front
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {0.0f,1.0f}, {0,0,1} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f}, {0.0f,0.0f}, {0,0,1} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {1.0f,0.0f}, {0,0,1} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {1.0f,1.0f}, {0,0,1} });
		// Back
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {0.0f,1.0f}, {0,0,-1} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {0.0f,0.0f} , {0,0,-1} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f}, {1.0f,0.0f}, {0,0,-1} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {1.0f,1.0f}, {0,0,-1} });
		// Right
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {0.0f,1.0f},{1,0,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {0.0f,0.0f},{1,0,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {1.0f,0.0f},{1,0,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {1.0f,1.0f},{1,0,0} });
		// Left
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {0.0f,1.0f},{-1,0,0} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f}, {0.0f,0.0f},{-1,0,0} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f}, {1.0f,0.0f},{-1,0,0} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {1.0f,1.0f},{-1,0,0} });
		// Top
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {0.0f,1.0f},{0,1,0} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {0.0f,0.0f},{0,1,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {1.0f,0.0f},{0,1,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {1.0f,1.0f},{0,1,0} });
		// Bottom
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f}, {0.0f,1.0f},{0,-1,0} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f}, {0.0f,0.0f},{0,-1,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {1.0f,0.0f},{0,-1,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {1.0f,1.0f},{0,-1,0} });
		break;
	}
	case SHAPE::PYRAMID:
	{
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f}, {0.0f,1.0f} } );
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f},{0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f},  {1.0f,0.0f} });

		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f},  {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f},{1.0f,0.0f} });

		// Top point / peak
		m_Vertices.emplace_back(Vertex{ {0.0f,  1.0f, 0.0f}, {0.5f,1.0f} });
		break;
	}
	case SHAPE::SPHERE:
	{
		GenerateSphereVertices(12);
		break;
	}
	default:
	{
		break;
	}
	}
}

void Mesh::CreateShapeIndices(SHAPE _shape)
{
	switch (_shape)
	{
	case SHAPE::CUBE:
	{
		if (m_WindingOrder == GL_CCW)
		{
			for (int i = 0; i < 6; i++)
			{
				m_Indices.emplace_back(4 * i);
				m_Indices.emplace_back((4 * i) + 1);
				m_Indices.emplace_back((4 * i) + 2);

				m_Indices.emplace_back(4 * i);
				m_Indices.emplace_back((4 * i) + 2);
				m_Indices.emplace_back((4 * i) + 3);
			}
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				m_Indices.emplace_back(4 * i);
				m_Indices.emplace_back((4 * i) + 2);
				m_Indices.emplace_back((4 * i) + 1);

				m_Indices.emplace_back(4 * i);
				m_Indices.emplace_back((4 * i) + 3);
				m_Indices.emplace_back((4 * i) + 2);
			}
		}

		break;
	}
	case SHAPE::PYRAMID:
	{
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(0);
		m_Indices.emplace_back(3);
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(3);
		m_Indices.emplace_back(2);

		m_Indices.emplace_back(4);
		m_Indices.emplace_back(5);
		m_Indices.emplace_back(12);
		m_Indices.emplace_back(6);
		m_Indices.emplace_back(7);
		m_Indices.emplace_back(12);
		m_Indices.emplace_back(8);
		m_Indices.emplace_back(9);
		m_Indices.emplace_back(12);
		m_Indices.emplace_back(10);
		m_Indices.emplace_back(11);
		m_Indices.emplace_back(12);
		break;
	}
	case SHAPE::SPHERE:
	{
		GenerateSphereIndices(12);
		break;
	}
	default:
	{
		break;
	}
	}
}

void Mesh::CreatePolygonVertices(unsigned int _numberOfSides)
{
	// If Its Got 4 Sides, Create Generic Quad
	if (_numberOfSides == 4)
	{
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.0f}, {0.0f,1.0f} }); // Top Left
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.0f}, {0.0f,0.0f} }); // Bottom Left
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.0f}, {1.0f,0.0f} }); // Bottom Right
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.0f}, {1.0f,1.0f} }); // Top Right
		return;
	}

	float angle = 0.0f, increment = (glm::two_pi<float>() / _numberOfSides);

	// Centre
	m_Vertices.emplace_back(Vertex{ {0.0f,  0.0f, 0.0f}, {0.5f,0.5f} });

	// Fan Around Centre
	float xPos, yPos;
	for (unsigned i = 0; i < _numberOfSides; i++)
	{
		xPos = cos(angle);
		yPos = sin(angle);
		m_Vertices.emplace_back(Vertex{ {xPos, yPos, 0 },{ToTexCoord(xPos),ToTexCoord(yPos)} });
		angle += increment;
	}
}

void Mesh::CreatePolygonIndices(unsigned int _numberOfSides)
{
	// If Its Got 4 Sides, Create Generic Quad Indices
	if (_numberOfSides == 4)
	{
		m_Indices.emplace_back(0);	// Top Left
		m_Indices.emplace_back(1);	// Bottom Left
		m_Indices.emplace_back(2);	// Bottom Right
		m_Indices.emplace_back(0);	// Top Left
		m_Indices.emplace_back(2);	// Bottom Right
		m_Indices.emplace_back(3);	// Top Right
		m_Indices.emplace_back(0);	// Top Left
		return;
	}

	// Generate Generalised Regular Polygon Indices
	for (unsigned i = 0; i < _numberOfSides; i++)
	{
		// Centre
		m_Indices.push_back(0);

		// Back At Beginning?
		if (i + 2 > _numberOfSides)
		{
			m_Indices.push_back(i + 1);
			m_Indices.push_back(1);
		}
		else
		{
			m_Indices.push_back(i + 1);
			m_Indices.push_back(i + 2);
		}
	}
}

void Mesh::CreateAndInitializeBuffers()
{
	// Vertex Array
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	// Vertex Buffer
	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

	// Index Buffer
	glGenBuffers(1, &m_IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

	// Layouts
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// TexCoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));
	// Normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normals)));
	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

float Mesh::ToTexCoord(float& _position)
{
	return (_position + 1) * 0.5f;
}

void Mesh::GenerateSphereVertices(int _fidelity)
{
	// Angles to keep track of the sphere points 
	float Phi = 0.0f;
	float Theta = 0.0f;

	// Cycle through x axis by increments of 1 / fidelity level
	for (int i = 0; i < _fidelity; i++)
	{
		// Starting angle of 0 for each y axis ring
		Theta = 0.0f;

		// Cycle through y axis by increments of 1 / fidelity level
		for (int j = 0; j < _fidelity; j++)
		{
			// Calculate new vertex positions based on 
			// current y axis angle and x axis angle
			float x = cos(Phi) * sin(Theta);
			float y = cos(Theta);
			float z = sin(Phi) * sin(Theta);

			// Add the new vertex point to the vertices vector
			m_Vertices.emplace_back(Vertex{
				{ x * 0.5f, y * 0.5f, z * 0.5f }, // Position
				{ 1 - (float)i / (_fidelity - 1), 1 - ((float)j / (_fidelity - 1)) }, // Texture coords
				{ x,y,z } // Normals
				});

			// update y axis angle by increments of PI / fidelity level
			// As the sphere is built ring by ring, 
			// the angle is only needed to do half the circumferance therefore using just PI
			Theta += (glm::pi<float>() / ((float)_fidelity - 1.0f));
		}

		// x and z axis angle is updated by increments of 2Pi / fidelity level
		// Angle uses 2*PI to get the full circumference as this layer is built as a full ring
		Phi += (glm::two_pi<float>()) / ((float)_fidelity - 1.0f);
	}
}

void Mesh::GenerateSphereIndices(int _fidelity)
{
	for (int i = 0; i < _fidelity; i++)
	{
		for (int j = 0; j < _fidelity; j++)
		{
			if (m_WindingOrder == GL_CCW)
			{
				// First triangle of the quad
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + ((j + 1) % _fidelity));
				m_Indices.emplace_back((i * _fidelity) + (j));
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + (j));

				// Second triangle of the quad
				m_Indices.emplace_back((i * _fidelity) + ((j + 1) % _fidelity));
				m_Indices.emplace_back((i * _fidelity) + (j));
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + ((j + 1) % _fidelity));
			}
			else
			{
				// First triangle of the quad
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + ((j + 1) % _fidelity));
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + (j));
				m_Indices.emplace_back((i * _fidelity) + (j));

				// Second triangle of the quad
				m_Indices.emplace_back((i * _fidelity) + ((j + 1) % _fidelity));
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + ((j + 1) % _fidelity));
				m_Indices.emplace_back((i * _fidelity) + (j));
			}
		}
	}
}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[i];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}

}

Mesh* Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// data to fill
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normals = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return new Mesh(vertices, indices, textures);
}

std::vector<Texture> Mesh::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < m_Textures.size(); j++)
		{
			if (std::strcmp(m_Textures[j].FilePath.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_Textures[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture = TextureLoader::LoadTexture(str.C_Str());
			textures.push_back(texture);
			m_Textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
		}
	}
	return textures;
}
