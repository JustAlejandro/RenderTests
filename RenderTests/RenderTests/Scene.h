#pragma once

#include "LightModel.h"

class Scene {
public:
	void loadModel(string path) {
		models.push_back(new Model(path));
	}
	void loadLightModel(string path, Light_Type light_type, vec3 dir, vec3 pos) {
		models.push_back(new LightModel(path, light_type, dir, pos));
	}
private:
	std::vector<Model*> models;
	std::vector<UniformSet*> uniforms;
};