#include "Mesh.h"
#include <iostream>

using namespace std;

//TODO: Implement this using the vertices/indices
void Mesh::draw()
{
	cout << vertices.size() << " " << indices.size() << " " << textures.size() << endl;
}

Mesh::Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::setupMesh() {

}
