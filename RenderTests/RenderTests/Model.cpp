#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

map<string, Model> Model::cache;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

void applyAll(vector<UniformSet*> unis, unsigned int shaderID) {
	for (UniformSet* u : unis) {
		u->apply(shaderID);
	}
}

void Model::draw()
{
	for (pair<unsigned int, Shader> shader : shaders) {
		shader.second.use();
		vector<UniformSet*> unis = meshes[shader.first].second;
		applyAll(unis, shader.second.id);
		for (Mesh* m : shader.second.meshes) {
            if (m->getShader() == shader.second.id) {
                m->draw(shader.second.id);
            }
		}
	}
}

void Model::bindShader(Shader s, int type)
{
    if (type == TYPE_ALL_OBJECTS) {
        for (map<uint, pair<vector<Mesh>, vector<UniformSet*>>>::iterator iter = meshes.begin(); iter != meshes.end(); iter++) {
            vector<Mesh>* allMesh = &iter->second.first;
            for (int i = 0; i < allMesh->size(); i++) {
                allMesh->at(i).setShader(s.id);
                s.meshes.push_back(&allMesh->at(i));
            }
        }
    }
	vector<Mesh>* meshPoint = &meshes[type].first;
	for (int i = 0; i < meshPoint->size(); i++)
	{
        meshPoint->at(i).setShader(s.id);
		s.meshes.push_back(&meshPoint->at(i));
	}
	shaders[type] = s;
}

void Model::bindUniform(UniformSet* u, int type)
{
    if (type == TYPE_ALL_OBJECTS) {
        for (map<uint, pair<vector<Mesh>, vector<UniformSet*>>>::iterator iter = meshes.begin(); iter != meshes.end(); iter++) {
            vector<UniformSet*>* allUni = &iter->second.second;
            allUni->push_back(u);
        }
    }
    else {
        meshes[type].second.push_back(u);
    }
}

Model Model::loadModel(string path) {
    map<string, Model>::iterator it = cache.find(path);
    if (it != cache.end()) return it->second;
    return Model(path);
}

Model::Model(string path) {

    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cerr << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    this->path = path.substr(0, path.find_last_of('/'));
    vector<Mesh> toType;
    processNode(toType, scene->mRootNode, scene);

    for (Mesh m : toType) {
        meshes[m.type].first.push_back(m);
    }
}

void Model::processNode(vector<Mesh>& meshes, aiNode* node, const aiScene* scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(meshes, node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.norm = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = vec;
        }
        else
            vertex.texCoord = vec2(0.0f, 0.0f);

        //// tangent
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.tan = vector;

        //// bitangent
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.bitan = vector;

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    vector<Texture> diffuseMaps = loadMaterialTextures(material,
        aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    vector<Texture> specularMaps = loadMaterialTextures(material,
        aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //The Model I'm using to test mislabels normal maps as height maps. Also assimp is at fault here.
    vector<Texture> normalMaps = loadMaterialTextures(material,
        aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    int type = TYPE_ALL_OBJECTS;
    if (diffuseMaps.size() == 0 && normalMaps.size() == 0 && specularMaps.size() == 0) type = TYPE_NO_TEXTURE;
    else if (diffuseMaps.size() > 0 && normalMaps.size() == 0 && specularMaps.size() == 0) type = TYPE_DIFFUSE_ONLY;
    else if (diffuseMaps.size() > 0 && normalMaps.size() > 0 && specularMaps.size() == 0) type = TYPE_DIFFUSE_BUMP;
    else if (diffuseMaps.size() > 0 && normalMaps.size() > 0 && specularMaps.size() > 0) type = TYPE_DIFFUSE_BUMP_SPECULAR;

    return Mesh(vertices, indices, textures, type);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), path);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma) {
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}