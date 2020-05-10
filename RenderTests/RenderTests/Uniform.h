#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <SDL_opengl.h>
#include <map>
#include <string>
#include <vector>

using namespace glm;
using namespace std;

enum Light_Type { LIGHT_TYPE_POINT, LIGHT_TYPE_DIRECTIONAL, LIGHT_TYPE_SPOTLIGHT, LIGHT_AMOUNT_OF_TYPES };

class UniformSet {
protected:
	UniformSet() = default;
public:
	virtual void apply(unsigned int shaderID) =0;
};

class GenericUniformSet : public UniformSet {
private:
	map<string, vec3*> vec3Uni;
	map<string, vec4*> vec4Uni;
	map<string, mat4*> mat4Uni;
	map<string, int*> intUni;
public:
	GenericUniformSet() = default;

	void set(string s, vec3* input) {
		vec3Uni[s] = input;
	}
	void set(string s, vec4* input) {
		vec4Uni[s] = input;
	}
	void set(string s, mat4* input) {
		mat4Uni[s] = input;
	}
	void set(string s, int* input) {
		intUni[s] = input;
	}
	void apply(unsigned int shaderID) {
		for (pair<string, vec3*> i : vec3Uni) {
			glUniform3fv(glGetUniformLocation(shaderID, i.first.c_str()), 1, (const GLfloat*)i.second);
		}
		for (pair<string, vec4*> i : vec4Uni) {
			glUniform4fv(glGetUniformLocation(shaderID, i.first.c_str()), 1, (const GLfloat*)i.second);
		}
		for (pair<string, mat4*> i : mat4Uni) {
			glUniformMatrix4fv(glGetUniformLocation(shaderID, i.first.c_str()), 1, GL_FALSE, (const GLfloat*)i.second);
		}
		for (pair<string, int*> i : intUni) {
			glUniform1i(glGetUniformLocation(shaderID, i.first.c_str()), *i.second);
		}
	}
};

class LightUniform {
public:
	virtual void apply(uint shaderID, int count[LIGHT_AMOUNT_OF_TYPES]) = 0;
protected:
	LightUniform() = default;
};

class LightUniformSet : public UniformSet {
public:
	void apply(unsigned int shaderID) {
		int count[LIGHT_AMOUNT_OF_TYPES] = { 0 };
		for (LightUniform* light : lights) {
			light->apply(shaderID, count);
		}
		// Applying the counts as uniforms
		for (int i = 0; i < LIGHT_AMOUNT_OF_TYPES; i++) {
			string name = "";
			switch (i)
			{
			case LIGHT_TYPE_POINT:
				name = "pointLightCount";
				break;
			case LIGHT_TYPE_DIRECTIONAL:
				name = "directionalLightCount";
				break;
			case LIGHT_TYPE_SPOTLIGHT:
				name = "spotLightCount";
				break;
			default:
				name = "noNameFound";
				break;
			}
			glUniform1i(glGetUniformLocation(shaderID,
				name.c_str()), count[i]);
		}
	}
	void add(LightUniform* light) {
		lights.push_back(light);
	}
private:
	vector<LightUniform*> lights;
};

class DirectionalLightUniform : public LightUniform {
public:
	DirectionalLightUniform(vec3* dir) {
		this->dir = dir;
	}
	void apply(unsigned int shaderID, int count[LIGHT_AMOUNT_OF_TYPES]) {
		glUniform3fv(glGetUniformLocation(shaderID,
			("directionalLights[" + std::to_string(count[LIGHT_TYPE_DIRECTIONAL]) + "].direction").c_str()), 1, (const GLfloat*)dir);
		count[LIGHT_TYPE_DIRECTIONAL]++;
	}
private:
	vec3* dir;
};

class SpotLightUniform : public LightUniform {
public:
	SpotLightUniform(vec3* pos, vec3* dir) {
		this->pos = pos;
		this->dir = dir;
	}
	void apply(unsigned int shaderID, int count[LIGHT_AMOUNT_OF_TYPES]) {
		glUniform3fv(glGetUniformLocation(shaderID,
			("spotLights[" + std::to_string(count[LIGHT_TYPE_SPOTLIGHT]) + "].direction").c_str()), 1, (const GLfloat*)dir);
		glUniform3fv(glGetUniformLocation(shaderID,
			("spotLights[" + std::to_string(count[LIGHT_TYPE_SPOTLIGHT]) + "].position").c_str()), 1, (const GLfloat*)pos);
		count[LIGHT_TYPE_SPOTLIGHT]++;
	}
private:
	vec3* dir;
	vec3* pos;
};

class PointLightUniform : public LightUniform {
public:
	PointLightUniform(vec3* pos) {
		this->pos = pos;
	}
	void apply(unsigned int shaderID, int count[LIGHT_AMOUNT_OF_TYPES]) {
		glUniform3fv(glGetUniformLocation(shaderID,
			("pointLights[" + std::to_string(count[LIGHT_TYPE_POINT]) + "].position").c_str()), 1, (const GLfloat*)&pos->x);
		count[LIGHT_TYPE_POINT]++;
	}
private:
	vec3* pos;
};
