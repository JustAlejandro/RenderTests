#include "ModelLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

Model ModelLoader::loadModel(string path)
{
	map<string, Model>::iterator it = cache.find(path);
	if (it != cache.end()) return it->second;
	
	return Model(path);
}
