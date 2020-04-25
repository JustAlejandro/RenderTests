#pragma once
#include "Mesh.h"
#include "Uniform.h"
#include "Shader.h"
#include <vector>
#include <assimp/scene.h>

using namespace std;



class Model
{
public:
	Model(string path);
	void processNode(vector<Mesh>& meshes, aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	//Holds the actual meshes, mapped by their type along with the associated uniforms
	map<unsigned int, pair<vector<Mesh>, vector<UniformSet*>>> meshes;
	//Cache textures so we don't load the same one multiple times
	vector<Texture> textures_loaded;
	//Path of the model for the cache to use
	string path;
	//Map of compiled shader programs, sorted by object type
	map<unsigned int, Shader> shaders;

	void draw();
	void bindShader(Shader s, int type);
	void bindUniform(UniformSet* u, int type);
};

