#pragma once
#include "Uniform.h"
#include <vector>

typedef struct Texture {
	uint id;
	string type;
	string path;
}Texture;

typedef struct Vertex {
	vec3 pos;
	vec3 norm;
	vec2 texCoord;
}Vertex;

class Mesh
{
public:
	unsigned int uid;
	int type;
	void draw();
	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures);
private:
	uint VAO, VBO, EBO;
	void setupMesh();
};

