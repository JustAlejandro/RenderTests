#pragma once
#include "Model.h"
#include "Light.h"

class LightModel : public Model {
public:
	LightModel(string path, Light_Type light_type, vec3 dir, vec3 pos) : Model(Model::loadModel(path)) {
		switch (light_type)
		{
		case LIGHT_TYPE_POINT:
			light = new PointLight(pos);
			break;
		case LIGHT_TYPE_DIRECTIONAL:
			light = new DirectionalLight(dir);
			break;
		case LIGHT_TYPE_SPOTLIGHT:
			light = new SpotLight(pos, dir);
			break;
		default:
			light = NULL;
			break;
		}
	}

	static LightModel loadModel(string path, Light_Type light_type, vec3 dir, vec3 pos) {
		return LightModel(path, light_type, dir, pos);
	}

	LightUniform* getLightUniform() {
		return light->getUniform();
	}
private:
	Light* light;
};