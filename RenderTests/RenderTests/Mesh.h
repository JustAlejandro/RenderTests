#pragma once
#include "Uniform.h"
#include <vector>

//Types of object's in the program. Also the ordering here is the order of rendering.
enum ObjectType { TYPE_ALL_OBJECTS, TYPE_NO_TEXTURE, TYPE_DIFFUSE_ONLY, TYPE_DIFFUSE_BUMP_SPECULAR, TYPE_DIFFUSE_BUMP };

typedef struct Texture {
	uint id;
	string type;
	string path;
}Texture;

typedef struct Vertex {
	vec3 pos;
	vec3 norm;
	vec2 texCoord;
	vec3 tan;
	vec3 bitan;
}Vertex;

class Mesh
{
public:
	unsigned int uid;
	int type;
	void draw(uint shaderID);
	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures, int type);
	void setShader(int s) { shaderID = s; }
	int getShader() { return shaderID; }
private:
	int shaderID;
	uint VAO, VBO, EBO;
	void setupMesh();
};

