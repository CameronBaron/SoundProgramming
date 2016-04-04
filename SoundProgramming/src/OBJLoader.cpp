#include "OBJLoader.h"
#include <assert.h>
#include <memory>
#include <fstream>

std::map<std::string, int> material_map;

OBJLoader::OBJLoader(const char* name, const char* objFilePath, const char* vertShaderfilePath, const char* fragShaderFilePath) :
	name(name), m_OBJFilePath(objFilePath), m_vertShaderFilePath(vertShaderfilePath), m_fragShaderFilePath(fragShaderFilePath)
{

}	

OBJLoader::~OBJLoader()
{
	shapes.clear();
	materials.clear();
	delete[] m_OBJFilePath;
}

void OBJLoader::Init()
{
	int success = GL_FALSE;
	unsigned int vertexShader = LoadShaderFromFile(m_vertShaderFilePath, GL_VERTEX_SHADER);
	unsigned int fragmentShader = LoadShaderFromFile(m_fragShaderFilePath, GL_FRAGMENT_SHADER);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);


	LoadFile();
	CreateBuffers();
}

void OBJLoader::LoadFile()
{
	std::string err;
	tinyobj::LoadObj(shapes, materials, err, m_OBJFilePath, "./data/");

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	std::cout << "# of shapes    : " << shapes.size() << std::endl;
	std::cout << "# of materials : " << materials.size() << std::endl;
	//PrintMeshInfo();
}

void OBJLoader::PrintMeshInfo()
{
	for (size_t i = 0; i < shapes.size(); i++) {
		printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
		printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
		printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
		assert((shapes[i].mesh.indices.size() % 3) == 0);
		for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
			printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3 * f + 0], shapes[i].mesh.indices[3 * f + 1], shapes[i].mesh.indices[3 * f + 2], shapes[i].mesh.material_ids[f]);
		}

		printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
		assert((shapes[i].mesh.positions.size() % 3) == 0);
		for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
			printf("  v[%ld] = (%f, %f, %f)\n", v,
				shapes[i].mesh.positions[3 * v + 0],
				shapes[i].mesh.positions[3 * v + 1],
				shapes[i].mesh.positions[3 * v + 2]);
		}
	}

	for (size_t i = 0; i < materials.size(); i++) {
		printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
		printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
		printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
		printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
		printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
		printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
		printf("  material.Ns = %f\n", materials[i].shininess);
		printf("  material.Ni = %f\n", materials[i].ior);
		printf("  material.dissolve = %f\n", materials[i].dissolve);
		printf("  material.illum = %d\n", materials[i].illum);
		printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
		printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
		printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
		printf("  material.map_Ns = %s\n", materials[i].bump_texname.c_str());
		//std::map::const_iterator it(materials[i].unknown_parameter.begin());
		//std::map::const_iterator itEnd(materials[i].unknown_parameter.end());
		//for (; it != itEnd; it++) {
		//	printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
		//}
		printf("\n");
	}
}

void OBJLoader::CreateBuffers()
{
	// Generate GL buffers
	gl_info.resize(shapes.size());
	for (unsigned int mesh_index = 0; mesh_index < shapes.size(); mesh_index++)
	{
		glGenVertexArrays(1, &gl_info[mesh_index].m_VAO);
		glGenBuffers(1, &gl_info[mesh_index].m_VBO);
		glGenBuffers(1, &gl_info[mesh_index].m_IBO);
		glBindVertexArray(gl_info[mesh_index].m_VAO);

		unsigned int float_count = shapes[mesh_index].mesh.positions.size();

		float_count += shapes[mesh_index].mesh.normals.size();
		float_count += shapes[mesh_index].mesh.texcoords.size();

		std::vector<float> vertex_data;
		vertex_data.reserve(float_count);

		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.positions.begin(), shapes[mesh_index].mesh.positions.end());
		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.normals.begin(), shapes[mesh_index].mesh.normals.end());
		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.texcoords.begin(), shapes[mesh_index].mesh.texcoords.end());

		gl_info[mesh_index].m_index_count = shapes[mesh_index].mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, gl_info[mesh_index].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float),
			vertex_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_info[mesh_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[mesh_index].mesh.indices.size() * sizeof(unsigned int),
			shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // Position
		glEnableVertexAttribArray(1); // Normal
		glEnableVertexAttribArray(2); // Texcoord
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float) * shapes[mesh_index].mesh.positions.size()));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * shapes[mesh_index].mesh.positions.size() + shapes[mesh_index].mesh.normals.size()));

		// Uniform
#pragma region Textures

		mat_ref = new unsigned int[materials.size()];
		int imageWidth = 0, imageHeight = 0, imageFormat = 0;
		//tinyobj::LoadMtl(material_map, materials, );

		glGenTextures(1, &mat_ref[0]);
		glBindTexture(GL_TEXTURE_2D, mat_ref[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &materials[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glActiveTexture(GL_TEXTURE0);
		unsigned int matTex = glGetUniformLocation(m_programID, "material");
		glUniform1i(matTex, 0);

		/*mat_ref = new unsigned int[materials.size()];

		for (int i = 0; i < materials.size(); i++)
		{
			int imageWidth = 0, imageHeight = 0, imageFormat = 0;

			glGenTextures(1, &mat_ref[i]);
			glBindTexture(GL_TEXTURE_2D, mat_ref[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &materials[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 1);
		unsigned int ambientTex = glGetUniformLocation(m_programID, "ambient");
		glUniform1i(ambientTex, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 1);
		unsigned int diffuseTex = glGetUniformLocation(m_programID, "diffuse");
		glUniform1i(diffuseTex, 0);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 1);
		unsigned int specularTex = glGetUniformLocation(m_programID, "specular");
		glUniform1i(specularTex, 0);*/

#pragma endregion

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}


	

}

void OBJLoader::DrawElements()
{
	glUseProgram(m_programID);
	
	for (unsigned int i = 0; i < gl_info.size(); i++)
	{
		glBindVertexArray(gl_info[i].m_VAO);
		//check_gl_error();
		glDrawElements(GL_TRIANGLES, gl_info[i].m_index_count, GL_UNSIGNED_INT, 0);
	}
}

unsigned int OBJLoader::LoadShaderFromFile(const char * _filePath, unsigned int _shaderType)
{
	FILE* file;
	fopen_s(&file, _filePath, "rb");
	if (file == nullptr)
		return -1;

	// Read the shader source
	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* source = new char[length + 1];
	memset(source, 0, length + 1);
	fread(source, sizeof(char), length, file);
	fclose(file);

	unsigned int shader = glCreateShader(_shaderType);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	delete[] source;

	return shader;
}