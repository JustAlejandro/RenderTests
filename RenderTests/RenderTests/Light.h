#pragma once
#include <glm/glm.hpp>
#include "Uniform.h"

using namespace glm;

class Light {
public:
	virtual LightUniform* getUniform() =0;
protected:
	Light() = default;
};

class PointLight : public Light {
public:
	PointLight(vec3 pos): uniform(&this->pos) {
		this->pos = pos;
	}

	LightUniform* getUniform() {
		return &uniform;
	}
	vec3 pos;
	PointLightUniform uniform;
};

class DirectionalLight : public Light {
public:
	DirectionalLight(vec3 dir): uniform(&this->dir) {
		this->dir = dir;
	}
	LightUniform* getUniform() {
		return &uniform;
	}
	vec3 dir;
	DirectionalLightUniform uniform;
};

class SpotLight : public Light {
public:
	SpotLight(vec3 pos, vec3 dir): uniform(&this->pos, &this->dir) {
		this->pos = pos;
		this->dir = dir;
	}
	LightUniform* getUniform() {
		return &uniform;
	}
	vec3 pos;
	vec3 dir;
	SpotLightUniform uniform;
};