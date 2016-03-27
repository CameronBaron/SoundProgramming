#pragma once
#include "tinyobjloader\tiny_obj_loader.h"
#include <iostream>
#include <vector>

class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	void LoadFile();
	void PrintMeshInfo();

	const char* m_OBJFilePath;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
};

