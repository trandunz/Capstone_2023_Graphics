#pragma once
#include "Mesh.h"
#include <unordered_map>

class StaticMesh
{
public:
	static std::map<int, Mesh*> MeshMap;
	static std::vector<Mesh*> Meshes;
};

