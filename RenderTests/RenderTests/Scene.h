#pragma once

#include "LightModel.h"

class Scene {
public:
	Scene() : camera() {};
	Model* loadModel(string name, string path) {
		Model* model = new Model(path);
		models.insert(std::make_pair(name,model));

		model->bindUniform(camera.getUniform(), TYPE_ALL_OBJECTS);
		return model;
	}
	LightModel* loadLightModel(string name, string path, Light_Type light_type, vec3 dir, vec3 pos) {
		LightModel* lightModel = new LightModel(path, light_type, dir, pos);
		models.insert(std::make_pair(name, lightModel));
		lights.add(lightModel->getLightUniform());

		lightModel->bindUniform(camera.getUniform(), TYPE_ALL_OBJECTS);
		return lightModel;
	}
	void bindShader(string name, Shader s, int type) {
		models[name]->bindShader(s, type);
	}
	void bindUniform(string name, UniformSet* u, int type) {
		models[name]->bindUniform(u, type);
	}
	void warpModel(string name, vec3 pos) {
		models[name]->warp(pos);
	}
	void moveModel(string name, vec3 pos) {
		models[name]->move(pos);
	}
	void scaleModel(string name, float size) {
		models[name]->scale(size);
	}
	void update() {
		camera.update();
	}
	void draw() {
		for (std::map<string, Model*>::iterator it = models.begin(); it != models.end(); it++) {
			it->second->draw();
		}
	}
	void bindBasicUniforms() {
		for (std::map<string, Model*>::iterator it = models.begin(); it != models.end(); it++) {
			it->second->bindUniform(camera.getUniform(), TYPE_ALL_OBJECTS);
			it->second->bindModelUniform();
		}
	}
	bool stillRunning() {
		return camera.stillRunning();
	}
	Camera camera;
private:
	// Using this to let us adress models by name
	std::map<string, Model*> models;
	std::vector<UniformSet*> uniforms;
	LightUniformSet lights;
};