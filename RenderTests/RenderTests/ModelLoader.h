#pragma once
#include "Model.h"
#include <map>
#include <string>
#include <assimp/scene.h>

using namespace std;

class ModelLoader
{
public:
	Model loadModel(string path);
private:
	map<string, Model> cache;
};

